# ES集群client master data设计思路

## 为什么需要设置master node、data node和client node

es集群里的master node、data node和client node到底是怎么个意思，分别有何特点？

 
**master节点**

主要功能是维护元数据，管理集群各个节点的状态，数据的导入和查询都不会走master节点，所以master节点的压力相对较小，因此master节点的内存分配也可以相对少些；但是master节点是最重要的，如果master节点挂了或者发生脑裂了，你的元数据就会发生混乱，那样你集群里的全部数据可能会发生丢失，所以一定要保证master节点的稳定性。

**data node**

是负责数据的查询和导入的，它的压力会比较大，它需要分配多点的内存，选择服务器的时候最好选择配置较高的机器（大内存，双路CPU，SSD... 土豪~）；data node要是坏了，可能会丢失一小份数据。

**client node**

是作为任务分发用的，它里面也会存元数据，但是它不会对元数据做任何修改。client node存在的好处是可以分担下data node的一部分压力；为什么client node能分担data node的一部分压力？因为es的查询是两层汇聚的结果，第一层是在data node上做查询结果汇聚，然后把结果发给client node，client node接收到data node发来的结果后再做第二次的汇聚，然后把最终的查询结果返回给用户；所以我们看到，client node帮忙把第二层的汇聚工作处理了，自然分担了data node的压力。
这里，我们可以举个例子，当你有个大数据查询的任务（比如上亿条查询任务量）丢给了es集群，要是没有client node，那么压力直接全丢给了data node，如果data node机器配置不足以接受这么大的查询，那么就很有可能挂掉，一旦挂掉，data node就要重新recover，重新reblance，这是一个异常恢复的过程，这个过程的结果就是导致es集群服务停止... 但是如果你有client node，任务会先丢给client node，client node要是处理不来，顶多就是client node停止了，不会影响到data node，es集群也不会走异常恢复。

对于es 集群为何要设计这三种角色的节点，也是从分层逻辑去考虑的，只有把相关功能和角色划分清楚了，每种node各尽其责，才能发挥出分布式集群的效果。

## 如何设置

在生产环境下，如果不修改elasticsearch节点的角色信息，在高数据量，高并发的场景下集群容易出现脑裂等问题。

　　默认情况下，elasticsearch 集群中每个节点都有成为主节点的资格，也都存储数据，还可以提供查询服务。这些功能是由两个属性控制的。
　　　　1. #### node.master

　　　　2. #### node.data
　　
　　默认情况下这两个属性的值都是true。

　　node.master：这个属性表示节点是否具有成为主节点的资格
　　注意：此属性的值为 true，并不意味着这个节点就是主节点。因为真正的主节点，是由多个具有主节点资格的节点进行选举产生的。所以，这个属性只是代表这个节点是不是具有主节点选举资格。

　　node.data：这个属性表示节点是否存储数据。

四种组合
　1. #### node.master: true AND node.data: true AND node.ingest: true

　　这种组合表示这个节点既有成为主节点的资格，又可以存储数据，还可以作为预处理节点，这个时候如果某个节点被选举成为了真正的主节点，那么他还要存储数据，这样对于这个节点的压力就比较大了。
　　elasticsearch 默认是：每个节点都是这样的配置，在测试环境下这样做没问题。实际工作中建议不要这样设置，这样相当于 主节点 和 数据节点 的角色混合到一块了。

　2. #### node.master: false AND node.data: true AND node.ingest: false

　　这种组合表示这个节点没有成为主节点的资格，也就不参与选举，只会存储数据。这个节点我们称为 data(数据)节点。在集群中需要单独设置几个这样的节点负责存储数据。后期提供存储和查询服务

　3. #### node.master: true AND node.data: false AND node.ingest: false

　　这种组合表示这个节点不会存储数据，有成为主节点的资格，可以参与选举，有可能成为真正的主节点。这个节点我们称为master节点

　4. #### node.master: false AND node.data: false AND node.ingest: true

　　这种组合表示这个节点即不会成为主节点，也不会存储数据，这个节点的意义是作为一个 client(客户端)节点，主要是针对海量请求的时候可以进行负载均衡。在新版 ElasticSearch5.x 之后该节点称之为：coordinate 节点，其中还增加了一个叫：ingest 节点，用于预处理数据（索引和搜索阶段都可以用到），当然，作为一般应用是不需要这个预处理节点做什么额外的预处理过程，那么这个节点和我们称之为 client 节点之间可以看做是等同的，我们在代码中配置访问节点就都可以配置这些 ingest 节点即可。

总结
　　默认情况下，每个节点都有成为主节点的资格，也会存储数据，还会处理客户端的请求。在一个生产集群中我们可以对这些节点的职责进行划分。

　　**建议集群中设置 3台 以上的节点作为 master 节点【node.master: true node.data: false node.ingest:false】，这些节点只负责成为主节点，维护整个集群的状态。**
　　**再根据数据量设置一批 data节点【node.master: false node.data: true node.ingest:false】，这些节点只负责存储数据，后期提供建立索引和查询索引的服务，这样的话如果用户请求比较频繁，这些节点的压力也会比较大**
　　**所以在集群中建议再设置一批 ingest 节点也称之为 client 节点【node.master: false node.data: false node.ingest:true】，这些节点只负责处理用户请求，实现请求转发，负载均衡等功能。**

　　**master节点：普通服务器即可(CPU 内存 消耗一般)**
　　**data   节点：主要消耗磁盘，内存**
　　**client | ingest  节点：普通服务器即可(如果要进行分组聚合操作的话，建议这个节点内存也分配多一点)**

