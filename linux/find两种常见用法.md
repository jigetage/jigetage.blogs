find两种常见用法

## 文件内容包含指定字符串

`find . | xargs grep string`#查找当前目录下文件内容包含字符串string的文件

为什么要这样写，因为管道命令符是把上一部的结果传递给下一步来处理，在 find . | grep string中虽然看似和find . | xargs grep string差不多，但是实际上还是有区别的。应为find .得到的结果是一串文件名集合，如果直接传递给grep的话，grep会把这些文件名看作一些无意义的字符串来处理。但是传递给xargs，他会把他当作一个有意义的文件来处理。

## 文件名包含指定字符串

```
find . -name '*string*'`#查找当前目录下文件名包含字符串string的文件
如果要查找文件名中不包含字符串string的文件，可以使用!（取非，也就是取反）。
`find . ! -name '*string*'
```