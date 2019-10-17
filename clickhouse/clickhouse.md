## 应用

目前已知国内在使用clickhouse的公司：新浪、海康威视、京东、58、腾讯、酷狗音乐等

## 特点

1，支持SQL

2，列式存储

3，支持在线查询，无延迟

4，yandex官方提供docker镜像，补充一点，yandex号称俄罗斯百度，是clickhouse开发和维护者，也是最大的使用者

5，中文社区 http://www.clickhouse.com.cn/

6，yandex官方提供中文文档 https://clickhouse.yandex/docs/zh/

7，支持分布式

8，采用c/s架构，支持httpapi和本地client

9，支持多种语言的library，包括golang nodejs，具体 https://clickhouse.yandex/docs/zh/interfaces/third-party/client_libraries/

10，自带可视化工具

11，官方支持导入mysql数据，https://clickhouse.yandex/docs/zh/database_engines/mysql/

12，官方支持接入kafka数据源

13，grafana支持接入clickhouse数据源

## 高性能和容灾

1，列式存储

2，数据压缩

3，多核心并行处理

4，支持分布式查询

5，支持向量引擎

6，支持近似查询

7，支持在线查询

8，支持索引

9，异步多主复制技术

## 不足之处

1，不支持事务操作

2，不支持delete update操作

3，稀疏索引使得ClickHouse不适合通过其键检索单行的点查询

## 比较

1，代替复杂的多样大数据技术组合架构，避免了使用多种框架和技术组合

2，减少维护成本

## 关键指标

6台4核8G内存的机器组成集群，select count(*) 3亿条数据 查询0.3 秒，3亿数据 group by 日期 查询 0.1 秒

官方提供的 100M 数据集的跑分结果：ClickHouse 比 Vertia 快约 5 倍，比 Hive 快 279 倍，比 My SQL 快 801 倍

虽然对不同的 SQL 查询，结果不完全一样，但是基本趋势是一致的

ClickHouse 跑分有多块？举个例子：ClickHouse 1 秒，Vertica 5.42 秒，Hive 279 秒

## 补充

OLAP系统关键特征

- 大多数是读请求

- 数据总是以相当大的批(> 1000 rows)进行写入

- 不修改已添加的数据

- 每次查询都从数据库中读取大量的行，但是同时又仅需要少量的列

- 宽表，即每个表包含着大量的列

- 较少的查询(通常每台服务器每秒数百个查询或更少)

- 对于简单查询，允许延迟大约50毫秒

- 列中的数据相对较小： 数字和短字符串(例如，每个URL 60个字节)

- 处理单个查询时需要高吞吐量（每个服务器每秒高达数十亿行）

- 事务不是必须的

- 对数据一致性要求低

- 每一个查询除了一个大表外都很小

- 查询结果明显小于源数据，换句话说，数据被过滤或聚合后能够被盛放在单台服务器的内存中

  补充：联机事务处理OLTP（on-line transaction processing）、联机分析处理OLAP（On-Line Analytical Processing）。





clickhouse

目前应用

目前已知国内在使用clickhouse的公司：新浪、海康威视、京东、58、腾讯、酷狗音乐等



目的

代替复杂的多样大数据技术组合架构，避免了使用多种框架和技术组合。

减少维护成本。



优点

1，支持sql

MySQL引擎用于将远程的MySQL服务器中的表映射到ClickHouse中，并允许您对表进行`INSERT`和`SELECT`查询，以方便您在ClickHouse与MySQL之间进行数据交换。

2，列式存储

没有垃圾数据，检索和查询时只关注有用数据，大大的减少io，可以把更多的数据加载到缓存中

3，支持在线查询，无延迟

4，yandex官方提供docker镜像，补充一点，yandex号称俄罗斯百度，是clickhouse开发和维护者

5，有中文社区 http://www.clickhouse.com.cn/

6，yandex官方提供中文文档 https://clickhouse.yandex/docs/zh/ 

clickhouse官方只支持4种语言的文档，俄语、英语、中文、阿拉伯语。

7，支持分布式，ClickHouse 中，数据可以驻留在不同的分片上。每个分片可以是用于容错的一组副本。查询在所有分片上并行处理

8，采用c/s架构，支持httpapi和本地client

9，支持多种语言的library，包括golang nodejs，具体 https://clickhouse.yandex/docs/zh/interfaces/third-party/client_libraries/

10，自带可视化工具

11，官方支持导入mysql数据，https://clickhouse.yandex/docs/zh/database_engines/mysql/

12，官方支持接入kafka数据源

13，grafana支持接入clickhouse数据源

13，是一个 DBMS，而不是一个单一的数据库。ClickHouse 允许在运行时创建表和数据库，加载数据和运行查询，而无需重新配置和重新启动服务器

14，数据是压缩过的

15，当进行一次很大的查询时，可以结合内存和磁盘一起查询

16，支持主键表，数据使用合并树 (MergeTree) 进行递增排序。由于这个原因，数据可以不断地添加到表中，并且添加数据不用加锁

17，支持近似计算 系统包含用于近似计算各种值，中位数和分位数的集合函数

18，支持根据部分样本数据进行查询，获得近似结果

19，采用多副本方式实现数据复制和容灾



缺点

1，不支持事务操作

2，不支持delete update操作

3，稀疏索引使得ClickHouse不适合通过其键检索单行的点查询



关键指标

6台4核8G内存的机器组成集群，select count(*) 3亿条数据 查询0.3 秒，3亿数据 group by 日期 查询 0.1 秒

官方提供的 100M 数据集的跑分结果：ClickHouse 比 Vertia 快约 5 倍，比 Hive 快 279 倍，比 My SQL 快 801 倍；虽然对不同的 SQL 查询，结果不完全一样，但是基本趋势是一致的。ClickHouse 跑分有多块？举个例子：ClickHouse 1 秒，Vertica 5.42 秒，Hive 279 秒



OLAP系统关键特征

- 大多数是读请求

- 数据总是以相当大的批(> 1000 rows)进行写入

- 不修改已添加的数据

- 每次查询都从数据库中读取大量的行，但是同时又仅需要少量的列

- 宽表，即每个表包含着大量的列

- 较少的查询(通常每台服务器每秒数百个查询或更少)

- 对于简单查询，允许延迟大约50毫秒

- 列中的数据相对较小： 数字和短字符串(例如，每个URL 60个字节)

- 处理单个查询时需要高吞吐量（每个服务器每秒高达数十亿行）

- 事务不是必须的

- 对数据一致性要求低

- 每一个查询除了一个大表外都很小

- 查询结果明显小于源数据，换句话说，数据被过滤或聚合后能够被盛放在单台服务器的内存中

  补充：联机事务处理OLTP（on-line transaction processing）、联机分析处理OLAP（On-Line Analytical Processing）。