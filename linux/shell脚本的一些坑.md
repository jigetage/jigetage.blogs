shell脚本的一些坑

1，赋值。通过=来进行赋值，但是一定注意，=的左右两侧都不能有空格，否则就错误了；

2，判断。通过=来进行两个变量的判断，切记啊，=的左右两侧都必须有空格，否则就错误了；

3，引用变量的值。通过${var}来引用变量的值，如果变量类型为字符串，那么一定注意要加上""来包围以下；

4，if语句。if语句的条件在[]中，切记，一定注意前后的中括号[]跟正文中间要各加一个空格；

附很好的脚本一个：

```
#! /usr/bin/env bash

echo "stop "

processnum=`ps -ef | grep hpcagent | awk '{if($3 == "1"){print $2}}'`

if [ "${processnum}" = "" ]; then
    echo "no hpcagent process running, no need to stop"
    exit
fi

echo "hpcagent process number is ${processnum}"

killall -9 hpcagent
if [ $? -ne 0 ]; then
    echo "killall command not found, try kill"
    # ps -ef | grep hpcagent | awk '{if($3 == "1"){print $2}}' | xargs kill -9
    kill -9 ${processnum}
    if [ $? -ne 0 ]; then
        echo "kill hpcagent process failed"
    else
        echo "kill hpcagent process successfully"
        echo "stop done"
    fi
else
    echo "kill hpcagent process successfully"
fi
```