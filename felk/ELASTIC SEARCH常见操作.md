<center>ELASTIC SEARCH常见操作</center>

## 一、基本概念

这里有一些Elasticsearch核心的基本概念。从一开始就理解这些概念将会对你接下来的学习有极大的帮助。

### 1.1 接近实时的

Near Realtime，简称NRT。Elasticsearch 是一个接近实时的搜索平台。这意味着从你添加一个索引`document`到它可以被搜索将会有一个轻微的延迟（通常是1秒）。

### 1.2 索引

一个索引（index）是一系列有类似特征的文档。例如，你可以为用户数据建立个索引，为产品目录建立另一个索引，再为订单数据创建另一个索引。一个索引使用一个名字唯一标识（必须全部小写）,并且这个名字也被用来查阅索引，在执行添加索引，搜索，更新，和删除操作时防止有文档在里面。

在一个集群里你想定义多少个索引都可以。

### 1.3 类型

在一个索引里，你可以定义一个或多个类型。一个类型是逻辑上的分类/划分，它的语义完全取决于你。总得来说，一个类型是为有一些共同域的文档（document）定义的。

### 1.4 文档

一个文档(document)是你可以索引的基本信息单位。例如，你可以有一个document储存一个单独的用户，另一个document储存单独的产品。这个document用JSON格式来表示。

在一个index/type里，你想存储多少个document都可以。注意尽管一个document物理上存在一个索引里，document实际上必须被indexed/assigned到索引里的一个类型。

### 1.5 切片与副本

一个索引可能存储大量的数据超出单台设备的存储上限。为了解决这个问题Elasticsearch支持把你的索引再分隔成多个切片叫做`shards`。当你创建一个索引时，你可以简单的定义你想要的切片数量。每一个切片在它内部都是一个功能完整和独立的"索引"，它可以被放置在集群里的任意一个节点。

切片有两个重要的功能：

- 它允许你水平切分你的内容体积
- 它允许你分发和并行操作

一旦被切分每个index都会有一些主切片和一些从切片（主切片的复制）。这些切片的数量可以在index被创建时一起定义。在索引被创建后，你可以在任何时候动态的改变从切片的数量，不能改变主切片的数量。

默认的Elasticsearch中的每个index被分配了5个主切片一份复制，这意味着你集群里有两个节点，你将拥有5个主切片和另外5个从切片（一个完整的复制）。每个index一共10个切片。

注意：
每个Elasticsearch切片是一个Lucene index，在一个独立的Lucene index里有一个最大document数：2,147,483,519 (= Integer.MAX_VALUE - 128)。你可以使用 `_cat/shards`api修改切片的容量。

## 二、安装

Elasticsearch 5.0最低要求Java版本是Java 8。我们推荐使用Oracle JDK version 1.8.0_73。Elasticsearch[下载地址](https://link.jianshu.com/?t=http://www.elastic.co/downloads)。这里用tar文件举例。

下载命令：

```bash
curl -L -O https://artifacts.elastic.co/downloads/elasticsearch/elasticsearch-5.0.2.tar.gz
```

然后解压：

```bash
tar -xvf elasticsearch-5.0.2.tar.gz
```

然后进入解压后的文件夹bin文件夹：

```bash
cd elasticsearch-5.0.2/bin
```

现在可以启动节点和单个集群（Windows 用户 运行 elasticsearch.bat）：

```bash
./elasticsearch
```

如果正常会看到以下内容：

```bash
[2016-09-16T14:17:51,251][INFO ][o.e.n.Node               ] [] initializing ...
[2016-09-16T14:17:51,329][INFO ][o.e.e.NodeEnvironment    ] [6-bjhwl] using [1] data paths, mounts [[/ (/dev/sda1)]], net usable_space [317.7gb], net total_space [453.6gb], spins? [no], types [ext4]
[2016-09-16T14:17:51,330][INFO ][o.e.e.NodeEnvironment    ] [6-bjhwl] heap size [1.9gb], compressed ordinary object pointers [true]
[2016-09-16T14:17:51,333][INFO ][o.e.n.Node               ] [6-bjhwl] node name [6-bjhwl] derived from node ID; set [node.name] to override
[2016-09-16T14:17:51,334][INFO ][o.e.n.Node               ] [6-bjhwl] version[5.0.2], pid[21261], build[f5daa16/2016-09-16T09:12:24.346Z], OS[Linux/4.4.0-36-generic/amd64], JVM[Oracle Corporation/Java HotSpot(TM) 64-Bit Server VM/1.8.0_60/25.60-b23]
[2016-09-16T14:17:51,967][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [aggs-matrix-stats]
[2016-09-16T14:17:51,967][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [ingest-common]
[2016-09-16T14:17:51,967][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [lang-expression]
[2016-09-16T14:17:51,967][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [lang-groovy]
[2016-09-16T14:17:51,967][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [lang-mustache]
[2016-09-16T14:17:51,967][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [lang-painless]
[2016-09-16T14:17:51,967][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [percolator]
[2016-09-16T14:17:51,968][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [reindex]
[2016-09-16T14:17:51,968][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [transport-netty3]
[2016-09-16T14:17:51,968][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded module [transport-netty4]
[2016-09-16T14:17:51,968][INFO ][o.e.p.PluginsService     ] [6-bjhwl] loaded plugin [mapper-murmur3]
[2016-09-16T14:17:53,521][INFO ][o.e.n.Node               ] [6-bjhwl] initialized
[2016-09-16T14:17:53,521][INFO ][o.e.n.Node               ] [6-bjhwl] starting ...
[2016-09-16T14:17:53,671][INFO ][o.e.t.TransportService   ] [6-bjhwl] publish_address {192.168.8.112:9300}, bound_addresses {{192.168.8.112:9300}
[2016-09-16T14:17:53,676][WARN ][o.e.b.BootstrapCheck     ] [6-bjhwl] max virtual memory areas vm.max_map_count [65530] likely too low, increase to at least [262144]
[2016-09-16T14:17:56,731][INFO ][o.e.h.HttpServer         ] [6-bjhwl] publish_address {192.168.8.112:9200}, bound_addresses {[::1]:9200}, {192.168.8.112:9200}
[2016-09-16T14:17:56,732][INFO ][o.e.g.GatewayService     ] [6-bjhwl] recovered [0] indices into cluster_state
[2016-09-16T14:17:56,748][INFO ][o.e.n.Node               ] [6-bjhwl] started
```

我们可以看到我们的节点叫做6-bjhwl（每个人遇到的名称都不相同）已经被启动并且选定它自己作为主节点在一个集群里。

当Elasticsearch启动时我们可以重写集群或者节点的名称：

```bash
./elasticsearch -Ecluster.name=my_cluster_name -Enode.name=my_node_name
```

同时也应该注意到输出里有http地址(192.168.8.112)和端口号(9200),我们的节点可以从这里访问。Elasticsearch默认使用9200端口来提供REST API访问，如果有必要它是可以配置的。

## 三、探索你的集群

现在我们有了自己的节点（和集群）并且运行起来了，下一步是理解如何与它交流。幸运的是，Elasticsearch提供了一个非常全面和强大的REST API，你可以通过它来与你的集群相互沟通。下面这些事情可以通过API来完成：

- 检查你的集群，节点和索引的健康情况，状态还可以统计
- 管理你的集群，节点，索引和元数据
- 对你的索引执行CRUD（增删改查）和检索操作
- 执行高级检索操作例如分页，排序，过滤，脚本，聚合等等

### 3.1 集群健康

让我们开始一次基本的健康检查，这样我们可以查看我们的集群正在干什么。

为了检查集群的健康，我们将会使用`_cat`API。

```undefined
GET /_cat/health?v
```

返回的结果是：

```css
epoch      timestamp cluster       status node.total node.data shards pri relo init unassign pending_tasks max_task_wait_time active_shards_percent
1475247709 17:01:49  elasticsearch green           1         1      0   0    0    0        0             0                  -                100.0%
```

我们可以看到我们的集群叫做"elasticsearch"，并且状态是绿色。

无论何时我们去请求集群的健康状态我们会得到：green, yellow, 或者 red。green意味着所有功能都是完好的，yellow意味着所有数据是可用的，但是一些副本还没有被分配，red代表一些数据由于某些原因已经不可用。注意，尽管一个集群是red状态，它仍然可以提供部分服务（比如，它会继续从可用的切片数据里搜索），但是在你失去部分数据后，你需要尽你最快的速度去修复它。

通过以下的API我们可以看到集群中的节点列表：

```undefined
GET /_cat/nodes?v
```

返回结果：

```css
ip        heap.percent ram.percent cpu load_1m load_5m load_15m node.role master         name
127.0.0.1           10           5   5    4.46                        mdi      *      PB2SGZY
```

可以看到我们的节点叫做"PB2SGZY"，目前我们集群里唯一的一个节点。

### 3.2 索引列表

现在让我们看一眼我们的索引：

```swift
GET /_cat/indices?v
```

返回如下：

```css
health status index uuid pri rep docs.count docs.deleted store.size pri.store.size
```

这表明我们还没有索引在集群中。

### 3.3 创建索引

现在让我们创建一个索引叫做"customer"：

```undefined
PUT /customer?pretty
```

通过PUT请求，我们创建了一个叫做"customer"的索引。`pretty`参数表示输出格式良好的JSON响应（如果存在）

然后再查看下索引列表：

```css
health status index    uuid                   pri rep docs.count docs.deleted store.size pri.store.size
yellow open   customer 95SQ4TSUT7mWBT7VNHH67A   5   1          0            0       260b           260b
```

我们这个"customer"索引有5个主切片和一份复制（默认设置），它里面有0个`document`。

你可能也注意到了"customer"索引的健康状态是yellow。yellow意味着一些副本还没有被分配。原因是目前集群里只有一个节点，副本暂时不能被分配（为了高可用性），直到另一个节点加入到集群中后。一旦副本被分配到另一个节点后这个索引的状态也将变为green。

### 3.4 索引与查询文档

让我们往"customer"索引里放点东西。记住，为了索引一个文档，我们必须告诉Elasticsearch这个文档的`type`。

让我们索引一个简单的customer文档到customer索引，`external`类型，ID是1：

```dart
PUT /customer/external/1?pretty
{
  "name": "John Doe"
}
```

返回结果：

```javascript
{
  "_index" : "customer",
  "_type" : "external",
  "_id" : "1",
  "_version" : 1,
  "result" : "created",
  "_shards" : {
    "total" : 2,
    "successful" : 1,
    "failed" : 0
  },
  "created" : true
}
```

重要的是，Elasticsearch并不要求你创建一个索引后才能向里面放入文档。Elasticsearch会自动创建"customer"索引，如果事先不存在的话。

现在查看下我们创建的文档：

```dart
GET /customer/external/1?pretty
```

返回如下：

```javascript
{
  "_index" : "customer",
  "_type" : "external",
  "_id" : "1",
  "_version" : 1,
  "found" : true,
  "_source" : { "name": "John Doe" }
}
```

### 3.5 删除一个索引

通过以下API：

```undefined
DELETE /customer?pretty
```

返回结果：

```css
health status index uuid pri rep docs.count docs.deleted store.size pri.store.size
```

下面总结下我们目前用到的一些API命令：

```dart
PUT /customer

PUT /customer/external/1
{
  "name": "John Doe"
}

GET /customer/external/1

DELETE /customer
```

我们可以发现以下规律：

```xml
<REST Verb> /<Index>/<Type>/<ID>
```

`<REST Verb>`是rest请求的类型，`<Index>`是索引名，`<Type>`是文档类型，`<ID>`是文档ID

## 四、修改你的数据

Elasticsearch提供了数据操纵和搜索的能力，几乎是实时的。从你`index/update/delete`数据到它出现在你的搜索结果里， 一般会有一秒的延迟（刷新间隔）。这个与其他平台像SQL数据库是一个重要的区别。

### 4.1 创建与替换文档

上面我们介绍了如何创建一个索引：

```dart
PUT /customer/external/1?pretty
{
  "name": "John Doe"
}
```

如果我们再次执行上面的命令，但是使用不同的文档，Elasticsearch将会把已经存在的ID为1的文档替换为新文档。

```dart
PUT /customer/external/1?pretty
{
  "name": "Tom"
}
```

如果我们使用一个不同的ID，那个一个新文档将被建立，ID为1的文档将不受影响。

创建文档时ID部分时可选的。如果不指定Elasticsearch将会生成一个随机的ID，这个ID在API的返回结果里可以看到。

下面是个例子：

```dart
POST /customer/external?pretty
{
  "name": "Jane Doe"
}
```

注意：如果不指定ID，使用的`POST`请求。

### 4.2 更新文档

如果创建文档时ID相同旧文档会被整体替换，我们也可以更新文档。注意实际上Elasticsearch并不在内部更新文档，不论何时你更新一个文档Elasticsearch是将旧文档删除然后创建一个更新后的新文档。

下面的例子展示了将ID为1的文档更新name字段，并且新增age字段：

```dart
POST /customer/external/1/_update?pretty
{
  "doc": { "name": "Jane Doe", "age": 20 }
}
```

还可以使用一些简单的脚本，下面我们把Jane Doe的年龄加5：

```dart
POST /customer/external/1/_update?pretty
{
  "script" : "ctx._source.age += 5"
}
```

在上面这个例子里，`ctx._source`引用的是当前将要被更新的文档。

### 4.3 删除文档

删除一个文档是相当简单的：

```dart
DELETE /customer/external/2?pretty
```

请查看[Delete By Query API](https://link.jianshu.com/?t=https://www.elastic.co/guide/en/elasticsearch/reference/5.0/docs-delete-by-query.html)，这里介绍了根据特定条件删除所有的文档。值得注意的是通过Delete By Query API删除所有的索引比删除所有文档要困难的多。

### 4.3 批处理

除了上面介绍的对单一文档进行操作外，Elasticsearch也提供了批量处理的能力，通过使用`_bulk`API。这个功能是很重要的，它提供了不同的机制来做多个操作，减少了与服务器直接来回传递数据的次数。

一个快速入门的例子，我们在一个批处理操作里创建两个文档：

```dart
POST /customer/external/_bulk?pretty
{"index":{"_id":"1"}}
{"name": "John Doe" }
{"index":{"_id":"2"}}
{"name": "Jane Doe" }

POST /customer/external/_bulk?pretty
{"update":{"_id":"1"}}
{"doc": { "name": "John Doe becomes Jane Doe" } }
{"delete":{"_id":"2"}}
```

bulk API会按照顺序依次执行相关操作，如果其中某个操作发生错误，剩下的操作也会继续执行。当bulk API返回时它会提供每个操作的状态（顺序与你发送时的顺序一样），这样你就可以检查每个操作是否成功。