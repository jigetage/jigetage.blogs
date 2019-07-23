直接上结论：

通过输出函数print来区别，即不加括号调用print就是python2，加括号调用print就是python3。

一般的python脚本作者很少在开头写清楚使用的python版本号。而python2和python3在很多语法上都是不同的。这一点可以通过print函数来分辨。

在python2中，print是作为语法结构出现的，调用时不需要加括号的，比如print 'hello world'。

而在python3中，print是作为函数出现的，调用时需要加括号，比如print('hello world')。