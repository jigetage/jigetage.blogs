golang中结构体方法如何进行去对象化处理

在有些场景更关心一组相似的操作：比如`Read`读取一些数组，然后调用`Close`关闭。此时的环境中，用户并不关心操作对象的类型，只要能满足通用的`Read`和`Close`行为就可以了。不过在方法表达式中，因为得到的`ReadFile`和`CloseFile`函数参数中含有`File`这个特有的类型参数，这使得`File`相关的方法无法和其它不是`File`类型但是有着相同`Read`和`Close`方法的对象无缝适配。这种小困难难不倒我们Go语言码农，我们可以通过结合闭包特性来消除方法表达式中第一个参数类型的差异：

```go
// 先打开文件对象
f, _ := OpenFile("foo.dat")

// 绑定到了 f 对象
// func Close() error
var Close = func() error {
    return (*File).Close(f)
}

// 绑定到了 f 对象
// func Read(offset int64, data []byte) int
var Read = func(offset int64, data []byte) int {
    return (*File).Read(f, offset, data)
}

// 文件处理
Read(0, data)
Close()
```

这刚好是方法值也要解决的问题。我们用方法值特性可以简化实现：

```go
// 先打开文件对象
f, _ := OpenFile("foo.dat")

// 方法值: 绑定到了 f 对象
// func Close() error
var Close = f.Close

// 方法值: 绑定到了 f 对象
// func Read(offset int64, data []byte) int
var Read = f.Read

// 文件处理
Read(0, data)
Close()
```