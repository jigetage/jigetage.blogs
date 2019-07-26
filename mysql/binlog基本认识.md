---
typora-copy-images-to: ../../../../Desktop/7928684-3959f914f4270b82.webp
---

##### binlog基本认识

MySQL中的binlog是一个二进制文件,它记录了所有的增删改操作。节点之间的复制就是依靠binlog来完成的。binlog具有三种模式:

- Row模式
   日志中会记录成每一行数据被修改的日志，然后在slave端再对相同的数据进行修改。例如:update xxx where id in(1,2,3,4,5);采用该模式则会记录5条记录。
- statement模式
   每一条会修改数据的sql都会记录到 master的binlog中。slave在复制的时候`sql Thread`会解析成和原来master端执行过的相同的sql来再次执行.
- mixed模式
   Mixed即混合模式,MySQL会根据执行的每一条具体的sql语句来区分对待记录的日志形式，也就是在Statement和Row之间选择一种。
   新版本中的Statment level还是和以前一样，仅仅记录执行的语句。而新版本的MySQL中队row level模式也被做了优化，并不是所有的修改都会以row level来记录，像遇到表结构变更的时候就会以statement模式来记录，如果sql语句确实就是update或者delete等修改数据的语句，那么还是会记录所有行的变更。

##### binlog之主从同步流程





![img](https://upload-images.jianshu.io/upload_images/7928684-3959f914f4270b82.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/794/format/webp)

binlog同步流程

 大体执行流程:

 Master执行完增删改操作后都会记录binlog日志,当需要同步的时候会主动通知slave节点,slave收到通知后使用

IO Thread主动去master读取binlog日志,然后异步写入relay日志(中转日志),然后使 SQL Thread完成对`relay日志  的解析然后入库操作,完成同步。