zookeeper与etcd的对比

### etcd

> `etcd`是一个高可用的键值存储系统，主要用于共享配置和服务发现。**etcd是由CoreOS开发并维护的，灵感来自于 ZooKeeper 和 Doozer，它使用Go语言编写，并通过Raft一致性算法处理日志复制以保证强一致性。Raft是一个来自Stanford的新的一致性算法，适用于分布式系统的日志复制，**Raft通过选举的方式来实现一致性，在Raft中，任何一个节点都可能成为Leader。**Google**的容器集群管理系统Kubernetes、开源PaaS平台Cloud Foundry和CoreOS的Fleet都广泛使用了etcd。

### zookeeper

> Zookeeper是一个用户维护配置信息、命名、分布式同步以及分组服务的集中式服务框架，它使用Java语言编写，通过([http://www.stanford.edu/class/cs347/reading/zab.pdf](https://links.jianshu.com/go?to=http%3A%2F%2Fwww.stanford.edu%2Fclass%2Fcs347%2Freading%2Fzab.pdf))协议来保证节点的一致性。因为Zookeeper是一个CP型(CAP中的CP)系统，所以当网络分区问题发生时，系统就不能注册或查找服务。

### etcd 与 zk 对比

|   比较项   | zookeeper |               etcd                |
| :--------: | :-------: | :-------------------------------: |
|    语言    |   java    |                go                 |
|  访问协议  |    tcp    |              restful              |
|  存储方式  | keyValue  |             keyValue              |
|  集群支持  | 1000+节点 |             1000+节点             |
|  分布式锁  | 简单稳定  |             实现复杂              |
|   watch    |   一般    | 一次watch操作就可以捕捉所有的事件 |
| 一致性算法 |    zab    |               raft                |