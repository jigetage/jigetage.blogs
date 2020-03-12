### init函数

init函数很特殊，一般函数不能命名为init函数，命名为init的函数不可以被调用，编译会出错

init函数不能有参数和返回值

同一个包里面，有很多文件，每个文件中可以出现一个或者多个init函数，这么多init函数的调用顺序是：

### 文件按照字母顺序从小到大依次调用，同一文件中出现的多个init函数，按照从上到下顺序依次调用

init函数常见用法：对于不能在初始化表达式中完成的初始化操作，可以引入init中进行

``

```go
var precomputed = [20]float64{}

func init() {
    var current float64 = 1
    precomputed[0] = current
    for i := 1; i < len(precomputed); i++ {
        precomputed[i] = precomputed[i-1] * 1.2
    }
}
```

使用 for 循环作为[*表达式*](https://golang.org/ref/spec#Expression)（Go 语言中的语句）是不可能的，所以将这些放到 init 函数中就能够很好的解决这些问题。