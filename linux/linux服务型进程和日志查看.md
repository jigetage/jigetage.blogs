# service

管理Linux系统服务

`service mysqld start/stop/restart/reload/stauts`

# chkconfig

`chkconfig`控制的是sysvinit进程，配置文件目录 cd /etc/rc.d/init.d

# systemctl

在centos7中，systemctl命令将service和chkconfig命令结合在了一起

`systemctl`控制的是systemd进程，配置文件目录 cd /usr/lib/systemd/system

常用命令 systemctl start/stop/restart/reload/status/list-unit-files/list-units/--failed/is-enabled

# journalctl

`journalctl `查看日志，参数如下：

`journalctl` 输出大量日志，有很多是无用的，因为是很早以前的日志

`journalctl -b` 限制其只输出本次启动的消息

 `journalctl -b -1` 查看自上次启动以来的全部消息。再上一次的？用 -2 替换 -1 吧。那自某个具体时间，例如2014年10月24日16:38以来的呢？

`journalctl -b --since=”2014-10-24 16:38”`

`journalctl -u gdm.service`  查看特定程序的日志

`journalctl _PID=890` 查看特定的进程号日志

`journalctl /usr/bin/pulseaudio` 只看某个可执行文件产生的消息



