docker-compose命令行参数解析



compose 常用命令解释
Usage: docker-compose [options] [options] [COMMAND] [ARGS...]
[options]

```shell
build
Usage: build [options] [--build-arg key=val...] [SERVICE...]
--no-cache 不使用缓存构建镜像
--build-arg key=val  设置构建时变量
//重新构建服务


config
Usage: :  config  [options]
-q, --quiet 只 验证不打印
--services 只 打印服务名称 ， 每行一个
--volumes 打印 数据 卷 名称 
//验证和查看Compose文件


exec
Usage: exec [options] SERVICE COMMAND [ARGS...]
-d 在后台运行命令
--privileged 给 这个进程赋予特权权限
-u, --user USER 作为该用户运行该命令
-T 禁用分配伪终端 ， 默认分配一个终端
--index=index 多个容器时的索引数字 ， 默认1
//在运行的容器里执行命令


port
Usage: port [options] SERVICE PRIVATE_PORT
--protocol=proto tcp 或udp ， 默认tcp
--index=index  多个容器时的索引数字 ， 默认1
//打印绑定的开放端口


ps
Usage: : s ps ] [options]  [SERVICE ...] ]
-q 只显示
//列出容器


rm
Usage: : rm[options][SERVICE ...]
-f, --force 强制 删除
-s, --stop 删除容器时如果需要先停止容器
-v 删除与容器相关的任何匿名卷
//删除停止的服务容器


scale
Usage: scale[options][SERVICE=NUM...]
//指定一个服务启动容器数量(常用）


up
Usage: up[options][--scale SERVICE=NUM...][SERVICE...]
-d 在后台运行容器
-no-deps 不启动连接服务
--no-recreate 如果容器存在，不重建他们
--no-build 不构建镜像，即使它丢失
--build 启动容器下构建镜像
--scale SERVICE=NUM 指定一个服务（容器）的启动数量
//创建和启动容器(常用）


stop
Usage:stop[SERVICE...]
//停止容器


start
Usage:start[SERVICE...]
//启动容器


restart
Usage:restart[options][SERVICE...]
//重启容器


top
Usage:top[SERVICE...]
//显示容器里运行的进程


logs
-f, --follow 实时输出日志
-t, --timestamps 显示时间戳
--tail="all" 从日志末尾显示行数
//显示容器的输出
```