日志分两类，一类是 Docker 引擎日志；另一类是 容器日志。

## Docker 引擎日志 

Docker 引擎日志 一般是交给了 Upstart(Ubuntu 14.04) 或者 systemd (CentOS 7, Ubuntu 16.04)。前者一般位于 /var/log/upstart/docker.log 下，后者一般通过 jounarlctl -u docker 来读取。不同系统的位置都不一样，SO上有人总结了一份列表，我修正了一下，可以参考：

系统	日志位置
Ubuntu(14.04)	/var/log/upstart/docker.log
Ubuntu(16.04)	journalctl -u docker.service
CentOS 7/RHEL 7/Fedora	journalctl -u docker.service
CoreOS	journalctl -u docker.service
OpenSuSE	journalctl -u docker.service
OSX	~/Library/Containers/com.docker.docker/Data/com.docker.driver.amd64-linux/log/d‌ocker.log
Debian GNU/Linux 7	/var/log/daemon.log
Debian GNU/Linux 8	journalctl -u docker.service
Boot2Docker	/var/log/docker.log

## 容器日志 

容器的日志 则可以通过 docker logs 命令来访问，而且可以像 tail -f 一样，使用 docker logs -f 来实时查看。如果使用 Docker Compose，则可以通过 docker-compose logs <服务名> 来查看。

如果深究其日志位置，每个容器的日志默认都会以 json-file 的格式存储于 /var/lib/docker/containers/<容器id>/<容器id>-json.log 下，不过并不建议去这里直接读取内容，因为 Docker 提供了更完善地日志收集方式 - Docker 日志收集驱动。

关于日志收集，Docker 内置了很多日志驱动，可以通过类似于 fluentd, syslog 这类服务收集日志。无论是 Docker 引擎，还是容器，都可以使用日志驱动。比如，如果打算用 fluentd 收集某个容器日志，可以这样启动容器：

$ docker run -d \
–log-driver=fluentd \
–log-opt fluentd-address=10.2.3.4:24224 \
–log-opt tag=”docker.{{.Name}}” \
nginx
其中 10.2.3.4:24224 是 fluentd 服务地址，实际环境中应该换成真实的地址。

dockerd 日志配置
要改变dockerd的日志为debug，可以重启dockerd，并提供启动参数-D或–debug，示例如下：

dockerd -D

dockerd的日志文件，默认为/var/log/messages，也可以重定向到其他日志文件，示例如下：

dockerd -D >> log_file 2>&1

方法二：
vi /etc/docker/daemon.json

{  
  "debug": true  
}
1
2
3
重启docker daemon

systemctl stop docker
systemctl daemon-reload
systemctl start docker

dockerd的日志级别
dockerd支持的日志级别debug, info, warn, error, fatal，默认的日志级别为info。必要的情况下，还需要设置日志级别，这也可以通过配置文件，或者通过启动参数-l或–log-level。
方法一：配置文件/etc/docker/daemon.json

{  
  "log-level": "debug"  
} 
1
2
3
方法二：

dockerd --log-level debug  
dockerd -l debug 
1
2
查看日志
方法一：

tail -f /var/log/messages

方法二：

watch -d -n 1 cat /var/log/messages

-d表示高亮不同的地方，-n表示多少秒刷新一次。
该指令，不会直接返回命令行，而是实时打印日志文件中新增加的内容，这一特性，对于查看日志是非常有效的。如果想终止输出，按 Ctrl+C 即可