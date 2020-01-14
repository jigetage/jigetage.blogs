as   可理解为：用作、当成，作为；一般是重命名列名或者表名。（主要为了查询方便）

（1）

如：表text，  列 column_1,column_2  你可以写成
 select  column_1  as  列1,column_2 as  列2   from  text as  表;
上面的语句可以解释为：选择 column_1  作为   列1,column_2 作为   列2  从 text  当成  表

（2）
select * from blog as B;
解析：查找所有blog 表里面的数据，并把blog表格命名为 B；

当你命名一个表之后，你可以在下面用 B 代替 blog；

select *from B;

二：as可以作为连接语句的操作符。
sql：create table  tablename as select * from tablename2;
解释：上面语句的意思就是先获取到tablename2表中的所有记录，之后创建一张tablename表，结构和tablename2表相同，记录为后面语句的查询结果。