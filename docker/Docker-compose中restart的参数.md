Docker-compose中restart包含4个参数

none on-failure always unless-stopped

none：无重启策略，关闭或者挂掉了就不再重启了

on-failures：只有当以非0状态结束时，即异常结束时，才会重启

always：不论是人为关闭，或者挂掉了，或者docker服务重启了，都会重启容器。特别的，不论在reboot之前容器是否运行，在reboot之后都会自动启动容器。

unless-stopped：大体上与always一样，差别是1，当reboot之前容器已经关闭了，那么reboot之后不会自动启动容器。2，如果docker服务关闭了，在重启docker服务之后也不会自动启动容器。

推荐使用方式

1，对于服务性质的，设置为always或者unless-stopped

2，对于计算一次性质的，设置为on-failures