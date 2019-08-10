flink日志文件解析

查看flink日志文件，全部如下所示：

 zhengxiao@mbp  ~/Documents/Codes/flink-1.8.1/log  ll
total 6032
-rw-r--r--  1 zhengxiao  staff   9.7K  8  9 15:15 flink-zhengxiao-client-mbp.log
-rw-r--r--  1 zhengxiao  staff   1.4M  8  9 13:15 flink-zhengxiao-standalonesession-0-mbp.log
-rw-r--r--  1 zhengxiao  staff     0B  8  8 17:47 flink-zhengxiao-standalonesession-0-mbp.out
-rw-r--r--  1 zhengxiao  staff    79K  8 10 00:00 flink-zhengxiao-standalonesession-1-mbp.log
-rw-r--r--  1 zhengxiao  staff   120K  8  9 23:58 flink-zhengxiao-standalonesession-1-mbp.log.1
-rw-r--r--  1 zhengxiao  staff   409K  8  9 23:53 flink-zhengxiao-standalonesession-1-mbp.log.2
-rw-r--r--  1 zhengxiao  staff   9.5K  8  8 17:50 flink-zhengxiao-standalonesession-1-mbp.log.3
-rw-r--r--  1 zhengxiao  staff     0B  8  9 23:58 flink-zhengxiao-standalonesession-1-mbp.out
-rw-r--r--  1 zhengxiao  staff     0B  8  9 23:55 flink-zhengxiao-standalonesession-1-mbp.out.1
-rw-r--r--  1 zhengxiao  staff     0B  8  9 14:56 flink-zhengxiao-standalonesession-1-mbp.out.2
-rw-r--r--  1 zhengxiao  staff     0B  8  8 17:50 flink-zhengxiao-standalonesession-1-mbp.out.3
-rw-r--r--  1 zhengxiao  staff    11K  8  9 23:58 flink-zhengxiao-taskexecutor-0-mbp.log
-rw-r--r--  1 zhengxiao  staff    12K  8  9 23:58 flink-zhengxiao-taskexecutor-0-mbp.log.1
-rw-r--r--  1 zhengxiao  staff    54K  8  9 23:53 flink-zhengxiao-taskexecutor-0-mbp.log.2
-rw-r--r--  1 zhengxiao  staff   105K  8  9 13:14 flink-zhengxiao-taskexecutor-0-mbp.log.3
-rw-r--r--  1 zhengxiao  staff     0B  8  9 23:58 flink-zhengxiao-taskexecutor-0-mbp.out
-rw-r--r--  1 zhengxiao  staff     0B  8  9 23:55 flink-zhengxiao-taskexecutor-0-mbp.out.1
-rw-r--r--  1 zhengxiao  staff   624B  8  9 23:53 flink-zhengxiao-taskexecutor-0-mbp.out.2
-rw-r--r--  1 zhengxiao  staff   611B  8  9 11:42 flink-zhengxiao-taskexecutor-0-mbp.out.3



先说一下，最后带有数字的扩展名指的是flink服务重新启动了，每次重启会生成新的standalonesession log/out以及standalonesession log/out文件。

中间的数字代表同一类日志文件，在同一个flink服务作用期限内，产生的时间顺序，数字越小代表产生的时间越早，可以使得一个文件不至于特别大。

可以看出，大概分为两类，一类是taskexecutor的log和out，另一类是standalonesession的log和out。

taskexecutor.log指的是flink的job启动日志，包含此job启动是否成功，请求端口等数据，对应web看板上的taskmanagers中指定task的logs

taskexecutor.out指的是flink job的输出，对应web看板上的taskmanagers中指定task的stdout

standalonesession.log指的是

