# 1.概述

创建Docker镜像的方式有三种

- docker commit命令：由容器生成镜像；
- Dockerfile文件+docker build命令；
- 从本地文件系统导入：OpenVZ的模板。

关于这三种方式的大致说明请参考[yeasy/docker_practice的创建镜像](https://github.com/yeasy/docker_practice/blob/master/image/create.md)。

最近学习了Dockerfile文件的相关配置，这里做一下简单的总结，并对之前一直感到有些迷惑的CMD和ENTRYPOINT指令做个差异对比。

# 2.Dockerfile文件总结

Dockerfile 由一行行命令语句组成，并且支持以 # 开头的注释行。

一般地，Dockerfile 分为四部分：基础镜像信息、维护者信息、镜像操作指令和容器启动时执行指令。

| 四部分             | 指令                     |
| ------------------ | ------------------------ |
| 基础镜像信息       | FROM                     |
| 维护者信息         | MAINTAINER               |
| 镜像操作指令       | RUN、COPY、ADD、EXPOSE等 |
| 容器启动时执行指令 | CMD、ENTRYPOINT          |

Dockerfile文件的第一条指令必须是FROM，其后可以是各种镜像的操作指令，最后是CMD或ENTRYPOINT指定容器启动时执行的命令。

下面引用[yeasy/docker_practice](https://github.com/yeasy/docker_practice/blob/master/dockerfile/instructions.md)对Dockerfile中各个指令的介绍，

> **指令**
>
> 指令的一般格式为 INSTRUCTION arguments，指令包括 FROM、MAINTAINER、RUN 等。
>
> **FROM**
>
> 格式为 FROM <image>或FROM <image>:<tag>。
>
> 第一条指令必须为 FROM 指令。并且，如果在同一个Dockerfile中创建多个镜像时，可以使用多个 FROM 指令（每个镜像一次）。
>
> **MAINTAINER**
>
> 格式为 MAINTAINER <name>，指定维护者信息。
>
> **RUN**
>
> 格式为 RUN <command> 或 RUN ["executable", "param1", "param2"]。
>
> 前者将在 shell 终端中运行命令，即 /bin/sh -c；后者则使用 exec 执行。指定使用其它终端可以通过第二种方式实现，例如 RUN ["/bin/bash", "-c", "echo hello"]。
>
> 每条 RUN 指令将在当前镜像基础上执行指定命令，并提交为新的镜像。当命令较长时可以使用 \ 来换行。
>
> **CMD**
>
> 支持三种格式
>
> ​    CMD ["executable","param1","param2"] 使用 exec 执行，推荐方式；
>
> ​    CMD command param1 param2 在 /bin/sh 中执行，提供给需要交互的应用；
>
> ​    CMD ["param1","param2"] 提供给 ENTRYPOINT 的默认参数；
>
> 指定启动容器时执行的命令，每个 Dockerfile 只能有一条 CMD 命令。如果指定了多条命令，只有最后一条会被执行。
>
> 如果用户启动容器时候指定了运行的命令，则会覆盖掉 CMD 指定的命令。
>
> **EXPOSE**
>
> 格式为 EXPOSE <port> [<port>...]。
>
> 告诉 Docker 服务端容器暴露的端口号，供互联系统使用。在启动容器时需要通过 -P，Docker 主机会自动分配一个端口转发到指定的端口。
>
> **ENV**
>
> 格式为 ENV <key> <value>。 指定一个环境变量，会被后续 RUN 指令使用，并在容器运行时保持。
>
> 例如
>
> ENV PG_MAJOR 9.3
>
> ENV PG_VERSION 9.3.4
>
> RUN curl -SL http://example.com/postgres-$PG_VERSION.tar.xz | tar -xJC /usr/src/postgress && …
>
> ENV PATH /usr/local/postgres-$PG_MAJOR/bin:$PATH
>
> **ADD**
>
> 格式为 ADD <src> <dest>。
>
> 该命令将复制指定的 <src> 到容器中的 <dest>。 其中 <src> 可以是Dockerfile所在目录的一个相对路径；也可以是一个 URL；还可以是一个 tar 文件（自动解压为目录）。
>
> **COPY**
>
> 格式为 COPY <src> <dest>。
>
> 复制本地主机的 <src>（为 Dockerfile 所在目录的相对路径）到容器中的 <dest>。
>
> 当使用本地目录为源目录时，推荐使用 COPY。
>
> **ENTRYPOINT**
>
> 两种格式：
>
> ​    ENTRYPOINT ["executable", "param1", "param2"]
>
> ​    ENTRYPOINT command param1 param2（shell中执行）。
>
> 配置容器启动后执行的命令，并且不可被 docker run 提供的参数覆盖。
>
> 每个 Dockerfile 中只能有一个 ENTRYPOINT，当指定多个时，只有最后一个起效。
>
> **VOLUME**
>
> 格式为 VOLUME ["/data"]。
>
> 创建一个可以从本地主机或其他容器挂载的挂载点，一般用来存放数据库和需要保持的数据等。
>
> **USER**
>
> 格式为 USER daemon。
>
> 指定运行容器时的用户名或 UID，后续的 RUN 也会使用指定用户。
>
> 当服务不需要管理员权限时，可以通过该命令指定运行用户。并且可以在之前创建所需要的用户，例如：RUN groupadd -r postgres && useradd -r -g postgres postgres。要临时获取管理员权限可以使用 gosu，而不推荐 sudo。
>
> **WORKDIR**
>
> 格式为 WORKDIR /path/to/workdir。
>
> 为后续的 RUN、CMD、ENTRYPOINT 指令配置工作目录。
>
> 可以使用多个 WORKDIR 指令，后续命令如果参数是相对路径，则会基于之前命令指定的路径。例如
>
> WORKDIR /a
>
> WORKDIR b
>
> WORKDIR c
>
> RUN pwd
>
> 则最终路径为 /a/b/c。
>
> **ONBUILD**
>
> 格式为 ONBUILD [INSTRUCTION]。
>
> 配置当所创建的镜像作为其它新创建镜像的基础镜像时，所执行的操作指令。
>
> 例如，Dockerfile 使用如下的内容创建了镜像 image-A。
>
> [...]
>
> ONBUILD ADD . /app/src
>
> ONBUILD RUN /usr/local/bin/python-build --dir /app/src
>
> [...]
>
> 如果基于 image-A 创建新的镜像时，新的Dockerfile中使用 FROM image-A指定基础镜像时，会自动执行 ONBUILD 指令内容，等价于在后面添加了两条指令。
>
> FROM image-A #Automatically run the followingADD . /app/srcRUN /usr/local/bin/python-build --dir /app/src
>
> 使用 ONBUILD 指令的镜像，推荐在标签中注明，例如 ruby:1.9-onbuild。

# 3.创建镜像

编写完Dockerfile文件后，通过运行docker build命令来创建自定义的镜像。Docker build命令格式如下：

> docker build [options] <path>
>
> 该命令将读取指定路径下（包括子目录）的 Dockerfile，并将该路径下所有内容发送给 Docker 服务端，由服务端来创建镜像。因此一般建议放置 Dockerfile 的目录为空目录。也可以通过 .dockerignore 文件（每一行添加一条匹配模式）来让 Docker 忽略路径下的目录和文件。

例如下面使用Dockerfile样例来创建了镜像test:0.0.1，其中-t选项用来指定镜像的tag。Dockerfile文件内容如下：

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
FROM ubuntu:14.04
MAINTAINER lienhua34@xxx.com

RUN mkdir /opt/leh
RUN touch /opt/leh/test

CMD echo "Hello lienhua34"
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

下面运行docker build命令生成镜像test:0.0.1，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
lienhua34@test$ sudo docker build -t test:0.0.1 .
Sending build context to Docker daemon 3.072 kB
Step 1 : FROM ubuntu:14.04
 ---> a5a467fddcb8
Step 2 : MAINTAINER lienhua34@163.com
 ---> Running in ce9e7b02f075
 ---> 332259a92e74
Removing intermediate container ce9e7b02f075
Step 3 : RUN mkdir /opt/leh
 ---> Running in e93f0a98040f
 ---> 097e177cf37f
Removing intermediate container e93f0a98040f
Step 4 : RUN touch /opt/leh/test
 ---> Running in f1531d3dea1a
 ---> 0f68852f8356
Removing intermediate container f1531d3dea1a
Step 5 : CMD echo "Hello lienhua34"
 ---> Running in cf3c5ce2af46
 ---> 811ce27ce692
Removing intermediate container cf3c5ce2af46
Successfully built 811ce27ce692
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

然后启动该镜像的容器来查看结果，

```shell
lienhua34@test$ sudo docker images
REPOSITORY                   TAG                 IMAGE ID            CREATED             VIRTUAL SIZE
test                         0.0.1               811ce27ce692        32 seconds ago      187.9 MB
lienhua34@test$ sudo docker run -ti test:0.0.1
Hello lienhua34
```

Dockerfile文件的每条指令生成镜像的一层（注：一个镜像不能超过127层）。Dockerfile中的指令被一条条地执行。每一步都创建一个新的容器，在容器中执行指令并提交修改。当所有指令执行完毕后，返回最终的镜像id。

# 4.Dockerfile文件中的CMD和ENTRYPOINT指令差异对比

CMD指令和ENTRYPOINT指令的作用都是为镜像指定容器启动后的命令，那么它们两者之间有什么各自的优点呢？

为了更好地对比CMD指令和ENTRYPOINT指令的差异，我们这里再列一下这两个指令的说明，

> **CMD**
>
> 支持三种格式
>
> ​    CMD ["executable","param1","param2"] 使用 exec 执行，推荐方式；
>
> ​    CMD command param1 param2 在 /bin/sh 中执行，提供给需要交互的应用；
>
> ​    CMD ["param1","param2"] 提供给 ENTRYPOINT 的默认参数；
>
> 指定启动容器时执行的命令，每个 Dockerfile 只能有一条 CMD 命令。如果指定了多条命令，只有最后一条会被执行。
>
> 如果用户启动容器时候指定了运行的命令，则会覆盖掉 CMD 指定的命令。
>
> **ENTRYPOINT**
>
> 两种格式：
>
> ​    ENTRYPOINT ["executable", "param1", "param2"]
>
> ​    ENTRYPOINT command param1 param2（shell中执行）。
>
> 配置容器启动后执行的命令，并且不可被 docker run 提供的参数覆盖。
>
> 每个 Dockerfile 中只能有一个 ENTRYPOINT，当指定多个时，只有最后一个起效。

从上面的说明，我们可以看到有两个共同点：

1. **都可以指定shell或exec函数调用的方式执行命令；**
2. **当存在多个CMD指令或ENTRYPOINT指令时，只有最后一个生效；**

而它们有如下差异：

​       **差异1：CMD指令指定的容器启动时命令可以被docker run指定的命令覆盖，而ENTRYPOINT指令指定的命令不能被覆盖，而是将docker run指定的参数当做ENTRYPOINT指定命令的参数。**

​       **差异2：CMD指令可以为ENTRYPOINT指令设置默认参数，而且可以被docker run指定的参数覆盖；**

下面分别对上面两个差异点进行详细说明，

## 4.1 差异1

> CMD指令指定的容器启动时命令可以被docker run指定的命令覆盖；而ENTRYPOINT指令指定的命令不能被覆盖，而是将docker run指定的参数当做ENTRYPOINT指定命令的参数。

下面有个命名为startup的可执行shell脚本，其功能就是输出命令行参数而已。内容如下所示，

```shell
#!/bin/bash

echo "in startup, args: $@"
```

### 通过CMD指定容器启动时命令：

现在我们新建一个Dockerfile文件，其将startup脚本拷贝到容器的/opt目录下，并通过CMD指令指定容器启动时运行该startup脚本。其内容如下，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
FROM ubuntu:14.04
MAINTAINER lienhua34@xxx.com

ADD startup /opt
RUN chmod a+x /opt/startup

CMD ["/opt/startup"]
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

然后我们通过运行docker build命令生成test:latest镜像，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
lienhua34@test$ sudo docker build -t test .
Sending build context to Docker daemon 4.096 kB
Step 1 : FROM ubuntu:14.04
 ---> a5a467fddcb8
Step 2 : MAINTAINER lienhua34@163.com
 ---> Using cache
 ---> 332259a92e74
Step 3 : ADD startup /opt
 ---> 3c26b6a8ef1b
Removing intermediate container 87022b0f30c5
Step 4 : RUN chmod a+x /opt/startup
 ---> Running in 4518ba223345
 ---> 04d9b53d6148
Removing intermediate container 4518ba223345
Step 5 : CMD /opt/startup
 ---> Running in 64a07c2f5e64
 ---> 18a2d5066346
Removing intermediate container 64a07c2f5e64
Successfully built 18a2d5066346
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

然后使用docker run启动两个test:latest镜像的容器，第一个docker run命令没有指定容器启动时命令，第二个docker run命令指定了容器启动时的命令为“/bin/bash -c 'echo Hello'”，

```shell
lienhua34@test$ sudo docker run -ti --rm=true test
in startup, args: 
lienhua34@test$ sudo docker run -ti --rm=true test /bin/bash -c 'echo Hello'
Hello
```

从上面运行结果可以看到，docker run命令启动容器时指定的运行命令覆盖了Dockerfile文件中CMD指令指定的命令。

### 通过ENTRYPOINT指定容器启动时命令：

将上面的Dockerfile中的CMD替换成ENTRYPOINT，内容如下所示，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
FROM ubuntu:14.04
MAINTAINER lienhua34@xxx.com

ADD startup /opt
RUN chmod a+x /opt/startup

ENTRYPOINT [“/opt/startup”]
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

同样，通过运行docker build生成test:latest镜像，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
lienhua34@test$ sudo docker build -t test .
Sending build context to Docker daemon 4.096 kB
Step 1 : FROM ubuntu:14.04
 ---> a5a467fddcb8
Step 2 : MAINTAINER lienhua34@163.com
 ---> Using cache
 ---> 332259a92e74
Step 3 : ADD startup /opt
 ---> Using cache
 ---> 3c26b6a8ef1b
Step 4 : RUN chmod a+x /opt/startup
 ---> Using cache
 ---> 04d9b53d6148
Step 5 : ENTRYPOINT /opt/startup
 ---> Running in cdec60940ad7
 ---> 78f8aca2edc2
Removing intermediate container cdec60940ad7
Successfully built 78f8aca2edc2
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

然后使用docker run启动两个test:latest镜像的容器，第一个docker run命令没有指定容器启动时命令，第二个docker run命令指定了容器启动时的命令为“/bin/bash -c 'echo Hello'”，

```shell
lienhua34@test$ sudo docker run -ti --rm=true test
in startup, args: 
lienhua34@test$ sudo docker run -ti --rm=true test /bin/bash -c 'echo Hello'
in startup, args: /bin/bash -c echo Hello
```

通过上面的运行结果可以看出，docker run命令指定的容器运行命令不能覆盖Dockerfile文件中ENTRYPOINT指令指定的命令，反而被当做参数传递给ENTRYPOINT指令指定的命令。

## 4.2 差异2

> CMD指令可以为ENTRYPOINT指令设置默认参数，而且可以被docker run指定的参数覆盖；

同样使用上面的startup脚本。编写Dockerfile，内容如下所示，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
FROM ubuntu:14.04
MAINTAINER lienhua34@xxx.com
 
ADD startup /opt
RUN chmod a+x /opt/startup

ENTRYPOINT ["/opt/startup", "arg1"]
CMD ["arg2"]
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

运行docker build命令生成test:latest镜像，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
lienhua34@test$ sudo docker build -t test .
Sending build context to Docker daemon 4.096 kB
Step 1 : FROM ubuntu:14.04
 ---> a5a467fddcb8
Step 2 : MAINTAINER lienhua34@163.com
 ---> Using cache
 ---> 332259a92e74
Step 3 : ADD startup /opt
 ---> Using cache
 ---> 3c26b6a8ef1b
Step 4 : RUN chmod a+x /opt/startup
 ---> Using cache
 ---> 04d9b53d6148
Step 5 : ENTRYPOINT /opt/startup arg1
 ---> Running in 54947233dc3d
 ---> 15a485253b4e
Removing intermediate container 54947233dc3d
Step 6 : CMD arg2
 ---> Running in 18c43d2d90fd
 ---> 4684ba457cc2
Removing intermediate container 18c43d2d90fd
Successfully built 4684ba457cc2
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

下面运行docker run启动两个test:latest镜像的容器，第一条docker run命令没有指定参数，第二条docker run命令指定了参数arg3，其运行结果如下，

```shell
lienhua34@test$ sudo docker run -ti --rm=true test
in startup, args: arg1 arg2
lienhua34@test$ sudo docker run -ti --rm=true test arg3
in startup, args: arg1 arg3
```

从上面第一个容器的运行结果可以看出CMD指令为ENTRYPOINT指令设置了默认参数；从第二个容器的运行结果看出，docker run命令指定的参数覆盖了CMD指令指定的参数。

## 4.3注意点

**CMD指令为ENTRYPOINT指令提供默认参数是基于镜像层次结构生效的，而不是基于是否在同个Dockerfile文件中。意思就是说，如果Dockerfile指定基础镜像中是ENTRYPOINT指定的启动命令，则该Dockerfile中的CMD依然是为基础镜像中的ENTRYPOINT设置默认参数。**

例如，我们有如下一个Dockerfile文件，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
FROM ubuntu:14.04
MAINTAINER lienhua34@xxx.com
 
ADD startup /opt
RUN chmod a+x /opt/startup

ENTRYPOINT ["/opt/startup", "arg1"]
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

通过运行docker build命令生成test:0.0.1镜像，然后创建该镜像的一个容器，查看运行结果，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
lienhua34@test$ sudo docker build -t test:0.0.1 .
Sending build context to Docker daemon 6.144 kB
Step 1 : FROM ubuntu:14.04
 ---> a5a467fddcb8
Step 2 : MAINTAINER lienhua34@163.com
 ---> Running in 57a96522061a
 ---> c3bbf1bd8068
Removing intermediate container 57a96522061a
Step 3 : ADD startup /opt
 ---> f9884fbc7607
Removing intermediate container 591a82b2f382
Step 4 : RUN chmod a+x /opt/startup
 ---> Running in 7a19f10b5513
 ---> 16c03869a764
Removing intermediate container 7a19f10b5513
Step 5 : ENTRYPOINT /opt/startup arg1
 ---> Running in b581c32b25c3
 ---> c6b1365afe03
Removing intermediate container b581c32b25c3
Successfully built c6b1365afe03
lienhua34@test$ sudo docker run -ti --rm=true test:0.0.1
in startup, args: arg1
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

下面新建一个Dockerfile文件，基础镜像是刚生成的test:0.0.1，通过CMD指定要通过echo打印字符串“in test:0.0.2”。文件内容如下所示，

```shell
FROM test:0.0.1
MAINTAINER lienhua34@xxx.com

CMD ["/bin/bash", "-c", "echo in test:0.0.2"]
```

运行docker build命令生成test:0.0.2镜像，然后通过运行docker run启动一个test:0.0.2镜像的容器来查看结果，

![复制代码](https://common.cnblogs.com/images/copycode.gif)

```shell
lienhua34@test$ sudo docker build -t test:0.0.2 .
Sending build context to Docker daemon 6.144 kB
Step 1 : FROM test:0.0.1
 ---> c6b1365afe03
Step 2 : MAINTAINER lienhua34@163.com
 ---> Running in deca95cf4c15
 ---> 971b5a819b48
Removing intermediate container deca95cf4c15
Step 3 : CMD /bin/bash -c echo in test:0.0.2
 ---> Running in 4a31c4652e1e
 ---> 0ca06ba31405
Removing intermediate container 4a31c4652e1e
Successfully built 0ca06ba31405
lienhua34@test$ sudo docker run -ti --rm=true test:0.0.2
in startup, args: arg1 /bin/bash -c echo in test:0.0.2
```

![复制代码](https://common.cnblogs.com/images/copycode.gif)

从上面结果可以看到，镜像test:0.0.2启动的容器运行时并不是打印字符串”in test:0.0.2”，而是将CMD指令指定的命令当做基础镜像test:0.0.1中ENTRYPOINT指定的运行脚本startup的参数。

(done)