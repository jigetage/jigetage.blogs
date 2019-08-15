docker的每个容器相当于有个内网地址。比如 mymysql 容器172.17.0.2，mynginx 容器为172.17.0.3 ，那么他们的通信机制是连到了 docker0 这个bridge，大概如下图：

![img](https://img2018.cnblogs.com/blog/1139553/201812/1139553-20181216104105119-688013375.png)

 

容器之间互相访问，一般形式是：ip+端口，比如：

```shell
// 进入容器：
docker run exec -it /bin/bash

// ping 一下端口发现是通的（得已经做了映射到本地）
ping 172.17.0.2:3306
```

—— 问题：每次部署或启动，相同容器的 IP 会发生变化。

—— 解决方法：docker中采用link来为容器起个名字，以后访问只要名字+端口就行了，IP 变化了也没关系。

 

**// link的使用方法：—— 请注意，这里只是介绍，请不要使用 link，将被废弃。**

第一种：使用 run 的参数



```
// 1、创建容器 test1
docker run -d --name test1 nginx

// 2、创建容器 test2并 link 到 test1。
docker run -d --name test2 --link test1 nginx

// 3、进入test2，并 ping test1，发现是可以 ping 通的。
docker exec -it test2 /bin/sh
ping test1
```



—— test2 link 到 test，其实就是修改了 test2自己的 host 文件和设置了环境变量而已。所以test2能 ping 通 test1，但是 test1无法 ping通 test2

 

第二种：在 yml 文件中的使用方法：

```shell
links:
 - db
 - db:database     //连接到 db 服务，并命名为 database
 - redis
```

使用的别名将会自动在服务容器中的/etc/hosts里创建。例如：

```shell
172.12.2.186  db
172.12.2.186  database
172.12.2.187  redis
```

—— 相应的环境变量也将被创建。

—— 使用 link 会默认连接到 【目录_defeat】 网络中，

 

 

 

—————— **关于使用 network** ——————

实际中是非常少使用 link 的，并且下一代版本将会被废弃。应该使用 network。

将容器连接到自己创建的同一个网络上，就可以相互 ping 通了。（记住，这里是自己创建的网桥才能 ping 通）

以下是常用的命令：



```shell
// 创建一个叫做 my-bridge 的网桥，使用的连接方式是 bridge
docker network create -d bridge my-bridge

// 显示所有 bridge，可以看到我们刚刚创建的 my-bridge 网络，id为75e4133b9ab2
docker network ls

// 创建容器mynginx2并添加到 my-bridge 网络中 
docker run --name mynginx2 --network my-bridge -p 8080:80 -d nginx:latest

// 使用 inspect 查看。NETWORK 显示的是 my-bridge，且 ID 为75e4133b9ab2。 内网 IP 为172.18.0.2（之前没指定都是默认的172.17.xx.xx，即默认的 docker0 ）
docker inspect mynginx2

——同理，你也可以通过 docker inspect my-bridge,会发现自建的 my-bridge 的 containers里面有 mynginx2

//查看 my-bridge 网络里面的容器
docker inspect my-bridge

//手动将某个容器加入网桥
docker network connect my-bridage test2

```



———— 使用 docker network ls 的时候，会发现，本来就有三中网络，分别为 host none bridge，并且这原始的三种是无法删除的、 

 

  

**// 在 yml 文件中定义networks：**



```yaml
version: '3'
services:
  nginx:
    image: nginx:1.15-alpine
    container_name: mynginx
    ports:
      - "80:80"
    volumes:
      - ${DIR_WWW}:${DIR_WWW}:rw
      - ./conf/nginx/conf.d:/etc/nginx/conf.d/:ro
      - ./conf/nginx/nginx.conf:/etc/nginx/nginx.conf:ro
      - ./log/nginx/:/var/log/nginx/:rw
    networks:
      - front

  phpfpm:
    image: raven666/ct-phpfpm:v1
    container_name: myphpfpm
    expose:
      - "9000"
    volumes:
      - ${DIR_WWW}:${DIR_WWW}:rw
      - ./conf/php/php.ini:/usr/local/etc/php/php.ini:ro
      - ./conf/php/php-fpm.d/www.conf:/usr/local/etc/php-fpm.d/www.conf:rw
      - ./conf/supervisor/conf.d:/etc/supervisor/conf.d/:ro
      - ./log/php-fpm/:/var/log/php-fpm/:rw
      - ./log/supervisor/:/var/log/supervisor/:rw
    command: supervisord -n
    networks:
      - front
      - backend
      
  mysql:
    image: mysql:5.7
    container_name: mymysql
    env_file: .env
    ports:
      - "3306:3306"
    volumes:
      - ./conf/mysql/mysql.cnf:/etc/mysql/conf.d/mysql.cnf:ro
      - ${DIR_MYSQL_DATA}:/var/lib/mysql/:rw
      - ./log/mysql/:/var/log/mysql/:rw
    environment:
      MYSQL_ROOT_PASSWORD: "${MYSQL_ROOT_PASSWORD}"
      MYSQL_DATABASE: "${MYSQL_DATABASE}"
    networks:
      - backend

  redis:
    image: redis:latest
    container_name: myredis
    env_file: .env
    command: redis-server --requirepass "${REDIS_PASSWORD}" --appendonly yes
    ports:
      - "6379:6379"
    volumes:
      - ${DIR_REDIS_DATA}:/data
    networks:
      - backend

networks:
  front:
  backend:
```



—— 因为我们指定了网络，所以up 之后，就会自动生成两个网络，分别为【当前目录名_front】和【当前目录名_backend】

—— 以上不指定 driver，则会默认使用 bridge。

—— 加入同一个网络的容器之间可以互联，比如以上加入front 的 nginx 和 phpfpm，它们就可以互联。同理，php 和 mysql，redis 都加入了 backend，也可以互联。

​       容器之间的访问可以使用【服务名:端口】的形式，或者【别名:端口】的形式。

那么，如何设置别名呢？如下例子：



```yaml
services:
  phpfpm:
    networks:
      front:
        aliases:
         - fpm
      backend:
        aliases:
         - fpm_back
```



—— phpfpm 在 front 网络中的别名叫做 fpm，所以加入 front 网络中的容器都可以使用以下两个方式访问：

1、【服务名:端口】   ==>>   【phpfpm:9000】  <<== 我们目前的 nginx 设置的就是这个。

2、【别名:端口】    ==>>    【fpm:9000】

—— 我目前一般没有设置别名，都用【服务名:端口】的形式访问。