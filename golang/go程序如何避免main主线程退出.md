go程序如何避免main主线程退出



我们可以根据控制Channel的缓存大小来控制并发执行的Goroutine的最大数目, 例如:

```go
var limit = make(chan int, 3)

func main() {
    for _, w := range work {
        go func() {
            limit <- 1
            w()
            <-limit
        }()
    }
    select{}
}
```

最后一句`select{}`是一个空的管道选择语句，该语句会导致`main`线程阻塞，从而避免程序过早退出。还有`for{}`、`<-make(chan int)`等诸多方法可以达到类似的效果。因为`main`线程被阻塞了，如果需要程序正常退出的话可以通过调用`os.Exit(0)`实现。