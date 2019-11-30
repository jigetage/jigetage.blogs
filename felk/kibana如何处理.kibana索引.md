**1，es升级注意的问题：**https://www.elastic.co/guide/en/elasticsearch/reference/6.5/rolling-upgrades.html

**2，kibana升级之后 原本保存的对象（dashboards, visualizations, index patterns）丢失**

迁移已运行后，在Elasticsearch会出现多个Kibana索引：.kibana_6, .kibana_5等。Kibana使用的是`.kibana（.kibana是其他索引的别名）所`指向的索引。

查看.kibana开头的索引有3个。.kibana  .kibana_1  .kibana_7

查看别名发现 .kibana是.kibana_1的别名

```shell
GET _alias
{
  ".kibana_1" : {
    "aliases" : {
      ".kibana" : { }
    }
  },
  ".kibana_7" : {
    "aliases" : { }
  }
}
```

分别查看.kibana_1 .kibana_7 的内容：

```shell
GET .kibana_7/_search
GET .kibana_1/_search
```

发现.kibana_7中存有原来的visualizations, index patterns数据，所以只需要将.kibana设置成.kibana_7的别名即可。

删除原来的别名：

```shell
POST _aliases
{
  "actions": [
    {
      "remove": {
        "index": ".kibana_1",
        "alias": ".kibana"
      }
    }
  ]
}
```

重新命名别名：

```shell
POST _aliases
{
  "actions": [
    {
      "add": {
        "index": ".kibana_7",
        "alias": ".kibana"
      }
    }
  ]
}
```

再查看当前.kibana索引中的内容，发现原来的数据存在，刷新页面，重启kibana，恢复。