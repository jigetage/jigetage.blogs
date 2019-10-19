常规写法

select * from table1 as t1

left/right/inner/outer join table2 as t2 on (t1.column1 = t2.column1 and t1.column2 = t2.column2)

where t1.column3 = condition and t2.column3 = condition

group by column

having column > value

order by t1.column4 desc/asc

limit 10



下面是例子分析
表A记录如下： 
aID        aNum 
1           a20050111 
2           a20050112 
3           a20050113 
4           a20050114 
5           a20050115 

表B记录如下: 
bID        bName 
1            2006032401 
2           2006032402 
3           2006032403 
4           2006032404 
8           2006032408 

创建这两个表SQL语句如下： 
CREATE TABLE  a 
aID int( 1 ) AUTO_INCREMENT PRIMARY KEY , 
aNum char( 20 ) 
) 
CREATE TABLE b( 
bID int( 1 ) NOT NULL AUTO_INCREMENT PRIMARY KEY , 
bName char( 20 )  
) 

INSERT INTO a 
VALUES ( 1, 'a20050111' ) , ( 2, 'a20050112' ) , ( 3, 'a20050113' ) , ( 4, 'a20050114' ) , ( 5, 'a20050115' ) ; 

INSERT INTO b 
VALUES ( 1, ' 2006032401' ) , ( 2, '2006032402' ) , ( 3, '2006032403' ) , ( 4, '2006032404' ) , ( 8, '2006032408' ) ; 

实验如下: 
1.left join(左联接) 

sql语句如下:  
SELECT * FROM a 
LEFT JOIN  b  
ON a.aID =b.bID 

结果如下: 
aID        aNum                   bID           bName 
1            a20050111         1               2006032401 
2            a20050112         2              2006032402 
3            a20050113         3              2006032403 
4            a20050114         4              2006032404 
5            a20050115         NULL       NULL 
（所影响的行数为 5 行） 

结果说明: 
        left join是以A表的记录为基础的,A可以看成左表,B可以看成右表,left join是以左表为准的. 
换句话说,左表(A)的记录将会全部表示出来,而右表(B)只会显示符合搜索条件的记录(例子中为: A.aID = B.bID). 
B表记录不足的地方均为NULL. 

2.right join(右联接) 

sql语句如下:  
SELECT  * FROM a 
RIGHT JOING b  
ON a.aID = b.bID 

结果如下: 
aID        aNum                   bID           bName 
1            a20050111         1               2006032401 
2            a20050112         2              2006032402 
3            a20050113         3              2006032403 
4            a20050114         4              2006032404 
NULL    NULL                   8              2006032408 
（所影响的行数为 5 行） 

结果说明: 
        仔细观察一下,就会发现,和left join的结果刚好相反,这次是以右表(B)为基础的,A表不足的地方用NULL填充. 

3.inner join(相等联接或内联接) 

sql语句如下:  
SELECT * FROM  a 
INNER JOIN  b 
ON a.aID =b.bID 

等同于以下SQL句: 
SELECT *  
FROM a,b 
WHERE a.aID = b.bID 

结果如下: 
aID        aNum                   bID           bName 
1            a20050111         1               2006032401 
2            a20050112         2              2006032402 
3            a20050113         3              2006032403 
4            a20050114         4              2006032404 

结果说明: 
        很明显,这里只显示出了 A.aID = B.bID的记录.这说明inner join并不以谁为基础,它只显示符合条件的记录. 
LEFT JOIN操作用于在任何的 FROM 子句中， 

组合来源表的记录。使用 LEFT JOIN 运算来创建一个左边外部联接。左边外部联接将包含了从第一个（左边）开始的两个表中的全部记录，即 
使在第二个（右边）表中并没有相符值的记录。 

语法：FROM table1 LEFT JOIN table2 ON table1.field1 compopr table2.field2  
说明：table1, table2参数用于指定要将记录组合的表的名称。 
field1, field2参数指定被联接的字段的名称。且这些字段必须有相同的数据类型及包含相同类型的数据，但它们不需要有相同的 
名称。 
compopr参数指定关系比较运算符："="， "<"， ">"， "<="， ">=" 或 "<>"。