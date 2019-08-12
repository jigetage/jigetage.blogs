在这篇文章中，您可以了解Filebeat的关键构建模块以及它们如何一起工作。了解这些概念将有助于您针对特定用例对Filebeat进行配置做出明智的决定。
Filebeat由两个主要组件组成： prospectors 和 harvesters。 这些组件一起工作来尾随文件并将事件数据发送到您指定的输出。

### 什么是harvesters

harvesters负责读取单个文件的内容。 harvesters逐行读取每个文件，并将内容发送到输出。 每个文件启动一台harvesters。 harvesters负责打开和关闭文件，这意味着在harvesters运行时文件描述符保持打开状态。 如果在收获文件时删除或重命名文件，Filebeat将继续读取文件。 这有副作用，在harvesters关闭之前，磁盘上的空间被保留。 默认情况下，Filebeat保持文件打开，直到达到close_inactive的设置（close_inactive默认为5分钟，即5分钟之内，没有最新的日志信息产生则关闭文件句柄）。
关闭harvester有以下情况：

- 如果在harvester还在读取文件时文件被删除，那么文件处理程序关闭，释放基础资源。
- 只有在scan_frequency过后，文件的采集才会重新开始。（scan_frequency参数默认为10秒，每隔10秒prospector检查目录中日志文件的变化情况）
- 如果在harvester关闭的情况下移动或移除文件，则不会继续收集文件。

### 什么是prospector

prospector负责管理harvesters并找到所有的读取源。如果输入类型是日志，则prospector会查找驱动器上与所定义的全局路径匹配的所有文件，并为每个文件启动一个harvesters。 每个prospector都在自己的Go例程中运行。
以下示例将Filebeat配置为从与指定的glob模式匹配的所有日志文件中获取行：

```
filebeat.prospectors:
    - type: log
      paths:
        - /var/log/*.log
        - /var/path2/*.log
```

Filebeat目前支持两种prospector类型：log和stdin。 每个prospector类型可以定义多次。 日志prospector检查每个文件以查看是否需要启动harvesters，是否已经运行，还是可以忽略文件（请参阅ignore_older，如果启用此选项，Filebeat将忽略在指定时间范围之前修改的任何文件。 如果长时间保存日志文件，配置ignore_older会特别有用。 例如，如果要启动Filebeat，但只想从上周发送最新的文件和文件，则可以配置此选项。）。 只有在harvesters关闭后文件的大小发生了变化的情况下，才会选择新行。
Filebeat的prospectors只能读取本地文件。 没有功能连接到远程主机读取存储的文件或日志。

### Filebeat如何保持文件的状态？

Filebeat保持每个文件的状态，并经常刷新注册表文件中的磁盘状态。状态用于记住收割机正在读取的最后偏移量，并确保发送所有日志行。

```
状态信息记录在/usr/local/filebeat-6.1.1/data目录中的registry文件
[root@vl010001134060 data]# pwd
/usr/local/filebeat-6.1.1/data
[root@vl010001134060 data]# ls
meta.json  registry
```

如果输出（如Elasticsearch或Logstash）无法访问，Filebeat将跟踪发送的最后一行，并在输出再次可用时继续读取文件。当Filebeat正在运行时，每个prospectors的状态信息也被保存在内存中。当Filebeat重新启动时，来自注册表文件的数据被用来重建状态，并且Filebeat继续在最后一个已知位置的每个harvester。
每个prospectors为每个找到的文件保留一个状态。由于文件可以被重命名或移动，文件名和路径不足以识别文件。对于每个文件，Filebeat存储唯一标识符以检测文件是否先前被收集。
如果您的使用案例涉及每天创建大量新文件，您可能会发现注册表文件会变得太大。（使用clean_inactive、clean_removed参数来调整）

### Filebeat如何确保至少一次交付？

Filebeat保证事件至少被传递到配置的输出一次，没有数据丢失。 Filebeat能够实现此行为，因为它将每个事件的传递状态存储在注册表文件中。
在定义的输出被阻止并且没有确认所有事件的情况下，Filebeat会一直尝试发送事件，直到输出确认已经收到事件。
如果Filebeat在发送事件的过程中关闭，则不会等待输出在关闭之前确认所有事件。 任何发送到输出的事件，在Filebeat关闭之前没有被确认，在重新启动Filebeat时会再次发送。 这可确保每个事件至少发送一次，但最终可能会将重复事件发送到输出。 您可以通过设置shutdown_timeout选项来配置Filebeat以在关闭之前等待特定时间。（shutdown_timeout选项默认是关闭状态，可以设置时间，关闭时等待多长时间后再关闭）。
但是如果日志写入磁盘的速度超过了Filebeat读取日志的速度，当日志删除或者日志被覆盖时，则可能会丢失数据。
例如：
在Linux文件系统上，Filebeat使用inode和设备来识别文件。从磁盘中删除文件时，可将inode分配给新文件。在涉及文件旋转的使用情况下，如果旧文件被删除并且之后立即创建新文件，则新文件可能与删除的文件具有完全相同的inode。在这种情况下，Filebeat假定新文件与旧文件相同，并尝试在旧位置继续读取，这是不正确的。
默认状态不会从注册表文件中删除。要解决inode重用问题，我们建议您使用clean_ *选项（特别是clean_inactive）来删除非活动文件的状态。例如，如果您的文件每24小时轮换一次，并且轮换的文件不再更新，则可以将ignore_older设置为48小时，将clean_inactive设置为72小时。
您可以使用clean_removed从磁盘中删除的文件。请注意，clean_removed会在扫描期间无法找到文件时清除注册表中的文件状态。如果该文件稍后再次显示，则将从头开始重新发送。