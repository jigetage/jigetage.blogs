prometheus+httpapi

1，query + instant time 获取的数据是一个瞬时值，其时间显示的是http请求返回的瞬时时间，而不是prometheus抓取数据的时间；

2，query + range time 获取的数据是数组，其元素时间显示的是prometheus抓取数据的具体时间，跟发起http请求无关；

3，query_range  获取的数据是数组，其元素时间是按照starttime开始，以此递加steptime的一组时间，且最后一个元素的时间不得超过endtime；