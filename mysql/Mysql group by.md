Mysql group by

字面上的意思就是分组，而事实上做的就是分组的工作，即将查到的数据按照列的值进行分组。

Group by column1, column2 表示先按照column1进行分组，然后在此基础上按照column2再进行分组。

可以和having 以及 order一起联合使用。

常见写法

select ... from table1 as t1 left/right/inner/outer join table2 as t2 on ... where ... group by ... having ... order by ... limit...



