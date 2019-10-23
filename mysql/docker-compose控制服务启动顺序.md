docker-compose控制服务启动顺序

# 参考

https://www.cnblogs.com/wang_yb/p/9400291.html

# 概要

docker-compose 可以方便组合多个 docker 容器服务, 但是, 当容器服务之间存在依赖关系时, docker-compose 并不能保证服务的启动顺序.

docker-compose 中的 depends_on 配置是容器的启动顺序, 并不是容器中服务的启动顺序.

# 问题重现

首先, 我们构造一个示例, 来演示 docker-compose 带来的问题. docker-compose.yml 文件如下:

```yaml
version: '2'
services:
  web:
    image: ubuntu:14.04
    depends_on:
      - web
    command: nc -z database 3306

  database:
    image: ubuntu:14.04
    command: >
      /bin/bash -c '
      sleep 5;
      echo "sleep over";
      nc -lk 0.0.0.0 3306;
      '
```

启动后, 可以发现, 确实是先启动 database, 后启动 web, 但是 database 中的服务是在大约 5 秒后才完成的, 所以导致 web 的启动失败.

```shell
$ docker-compose up
Creating tmp_database_1 ... done
Creating tmp_database_1 ...
Creating tmp_web_1      ... done
Attaching to tmp_database_1, tmp_web_1
tmp_web_1 exited with code 1
database_1  | sleep over
```

# 问题解决方式 1.0

修改 web 的启动脚本, 等待 database 的端口通了之后再启动服务

```shell
version: '2'
services:
  web:
    image: ubuntu:14.04
    depends_on:
      - database
    command: >
      /bin/bash -c '
      while ! nc -z database 3306;
      do
        echo "wait for database";
        sleep 1;
      done;

      echo "database is ready!";
      echo "start web service here";
      '

  database:
    image: ubuntu:14.04
    command: >
      /bin/bash -c '
      sleep 5;
      echo "sleep over";
      nc -lk 0.0.0.0 3306;
      '
```

再次启动,

```shell
$ docker-compose up
Creating tmp_database_1 ... done
Creating tmp_database_1 ...
Creating tmp_web_1      ... done
Attaching to tmp_database_1, tmp_web_1
web_1       | wait for database
web_1       | wait for database
web_1       | wait for database
web_1       | wait for database
web_1       | wait for database
database_1  | sleep over
web_1       | database is ready!
web_1       | start web service here
tmp_web_1 exited with code 0
```

web 会在 database 启动完成, 端口通了之后才启动.

# 问题解决方式 2.0

上面的解决方式虽然能够解决问题, 但是在 yaml 中直接插入脚本不好维护, 也容易出错. 如果有多个依赖, 或者多层依赖的时候, 复杂度会直线上升.

所以, 要封装一个 entrypoint.sh 脚本, 可以接受启动命令, 以及需要等待的服务和端口. 脚本内容如下:

```shell
#!/bin/bash
#set -x
#******************************************************************************
# @file    : entrypoint.sh
# @author  : wangyubin
# @date    : 2018-08- 1 10:18:43
#
# @brief   : entry point for manage service start order
# history  : init
#******************************************************************************

: ${SLEEP_SECOND:=2}

wait_for() {
    echo Waiting for $1 to listen on $2...
    while ! nc -z $1 $2; do echo waiting...; sleep $SLEEP_SECOND; done
}

declare DEPENDS
declare CMD

while getopts "d:c:" arg
do
    case $arg in
        d)
            DEPENDS=$OPTARG
            ;;
        c)
            CMD=$OPTARG
            ;;
        ?)
            echo "unkonw argument"
            exit 1
            ;;
    esac
done

for var in ${DEPENDS//,/ }
do
    host=${var%:*}
    port=${var#*:}
    wait_for $host $port
done

eval $CMD
```

这个脚本有 2 个参数, -d 需要等待的服务和端口, -c 等待的服务和端口启动之后, 自己的启动命令

修改 docker-compose.yml, 使用 entrypoint.sh 脚本来控制启动顺序.

```shell
version: '2'
services:
  web:
    image: ubuntu:14.04
    depends_on:
      - database
    volumes:
      - "./entrypoint.sh:/entrypoint.sh"
    entrypoint: /entrypoint.sh -d database:3306 -c 'echo "start web service here"';

  database:
    image: ubuntu:14.04
    command: >
      /bin/bash -c '
      sleep 5;
      echo "sleep over";
      nc -lk 0.0.0.0 3306;
      '
```

实际使用中, 也可以将 entrypoint.sh 打包到发布的镜像之中, 不用通过 volumes 配置来加载 entrypoint.sh 脚本.

测试结果如下:

```shell
$ docker-compose up
Starting tmp_database_1 ... done
Starting tmp_web_1 ... done
Attaching to tmp_database_1, tmp_web_1
web_1       | Waiting for database to listen on 3306...
web_1       | waiting...
web_1       | waiting...
web_1       | waiting...
database_1  | sleep over
web_1       | start web service here
tmp_web_1 exited with code 0
```

# 补充

## 依赖多个服务和端口

使用上面的 entrypoint.sh 脚本, 也可以依赖多个服务和端口, -d 参数后面的多个服务和端口用逗号(,)隔开.

```shell
version: '2'
services:
  web:
    image: ubuntu:14.04
    depends_on:
      - mysql
      - postgresql
    volumes:
      - "./entrypoint.sh:/entrypoint.sh"
    entrypoint: /entrypoint.sh -d mysql:3306,postgresql:5432 -c 'echo "start web service here"';

  mysql:
    image: ubuntu:14.04
    command: >
      /bin/bash -c '
      sleep 4;
      echo "sleep over";
      nc -lk 0.0.0.0 3306;
      '
  postgresql:
    image: ubuntu:14.04
    command: >
      /bin/bash -c '
      sleep 8;
      echo "sleep over";
      nc -lk 0.0.0.0 5432;
      '
```

执行的效果可以自行尝试.

## 尝试间隔的配置

每次尝试连接的等待时间可以通过 环境变量 SLEEP_SECOND 来配置, 默认 2 秒 下面的配置等待时间设置为 4 秒, 就会每隔 4 秒才去尝试 mysql 服务时候可连接.

```shell
version: '2'
services:
  web:
    image: ubuntu:14.04
    environment:
      SLEEP_SECOND: 4
    depends_on:
      - mysql
    volumes:
      - "./entrypoint.sh:/entrypoint.sh"
    entrypoint: /entrypoint.sh -d mysql:3306 'echo "start web service here"';

  mysql:
    image: ubuntu:14.04
    command: >
      /bin/bash -c '
      sleep 4;
      echo "sleep over";
      nc -lk 0.0.0.0 3306;
      '
```