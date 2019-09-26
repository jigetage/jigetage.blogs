kafka配置解析

docker中kafka文件位置/opt/kafka

docker中kafka数据位置/kafka/kafka-logs-kafkaservername

文件内容解析

kafka存储日志的路径是通过配置文件中的 log.dirs指定的，可以通过逗号分隔指定多个目录，也可以通过 log.dir来指定，作用一样。如果是实验环境，通常只需要指定一个目录，但是如果在生产环境，一般会指定多个，通过写入不同的磁盘来分散并行写的压力。

KafkaServer启动后，会在每个log.dirs的目录下，产生一堆文件：
2.1 .lock 文件，KafkaServer启动时会尝试去锁定它（如果没有该文件就创建），用于避免其他KafkaServer的实例共享同一目录
2.2 recovery-point-offset-checkpoint文件，记录了当前log目录下各个TopicPartition可用于恢复的offset信息，从这个offset开始后面的message都还木有flush到disk。
2.3 meta.properties文件，记录了版本号和当前Server实例的broker id，这个broker id会跟zookeeper上的信息进行关联，而且在后续的竞选Controller和各种协作中发挥作用。此文件中的broker id必须要跟配置文件里面的broker id一样，如果log.dirs指定了多个目录，则每个目录下这个文件的broker id必须一样，否则就会启动出错。如果配置文件没有指定broker id但允许自动生成，则会从zookeeper上通过sequenceNumber来生成它。
2.4 log-start-offset-checkpoint文件，日志可以返回给Client的最开始边界，对应earliest offset，最初始的值为0，如果日志被定期删除，则会被update成大于0的值。
2.5 cleaner-offset-checkpoint文件。（todo）
2.6 .kafka_cleanshutdown文件，标志Server的退出是正常的，下一次启动的时候会检查是否存在它，如果存在的话，就可以跳过一些恢复的环节。但是如果有些LogSegment在自检查的时候发现有错，还是会进入对应的恢复过程的。
2.7 replication-offset-checkpoint文件。（todo）

TopicPartition目录。创建Topic的时候，如果log.dirs存在多个路径，则会从中选中一个作为存储路径。如果Topic的partition大于1，则会存在多个TopicPartition目录，目录命名格式：topicName-partitionNumber。TopicPartition目录下会存在多组文件。这里说多组，是因为存储时是以Segment方式进行的，而每个Segment都会对应一组（几个相关的）文件

3.1 leader-epoch-checkpoint文件，对应整个log目录，(todo)
3.2 *.log文件，实际存储data的Segment文件，文件名是Segment的baseOffset（起始offset）。通过配置文件中的log.segment.bytes选项，可以定义当文件多大时roll out一个出新的Segment。
3.3 *.index文件，文件名是Segment的baseOffset，存储了messageRelativeOffset->filePosition的key-value list，通过配置，可以定义data中隔多少bytes就打一次index（bytes都是完整的记录包，所以不一定是准确的间隔），默认是4096 bytes。kafka partition中的offset都称为messageOffset, 而index文件中的messageRelativeOffset，则是通过 (messasgeOffset - Segment的baseOffset) 得到的，而filePosition就是message存储在log文件中的字节偏移量。
3.4 *.timeindex文件，文件名是Segment的baseOffset，存储了每个记录包中最大的timestamp -> 每个记录包中最大的messageRelativeOffset（存储时也是减去Segment的baseOffset）的key-value list。
3.5 *.snapshot文件，记录了producer的事务信息。（todo）
3.6 *.swap文件，用于Segment恢复。（todo）
3.7 *.txnindex文件，记录了中断的事务信息。(todo)