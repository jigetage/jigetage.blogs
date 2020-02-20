linux修改系统时间并且重启后依然生效

**修改配置文件来修改时区：**

　　1、修改vi /etc/sysconfig/clock    ZONE=‘Asia/Shanghai’

　　2、rm /etc/localtime

　　3、链接到上海时区文件： ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime