go编发编程的几个要点

1，如何保证goroutine安全退出？

通过select语法，在goroutine的主函数中传递一个chan对象，然后select 中有一个条件就是<-chan，执行语句是return。当goroutine外向chan写入时，就可以执行select中的<-chan分支，然后return了

2，如何保证多个goroutine安全退出？

通过select语法，在多个goroutine的主函数中传递同一个chan对象，然后select 中有一个条件就是<-chan，执行语句是return。当goroutine外，执行close(cancel)时，就可以执行select中的<-chan分支，然后return了

3，如何保证goroutine在main.main退出之前退出？

开启goroutine前，先创建一个WaitGroup对象，每开启一个goroutine前往wg中add 1，执行结束之后wg done以此，在main.main最后执行wg.Wait()

4，context.Context

通过ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)创建一个带有超时的context，在另外一处通过select语法，其中某个case的条件时

```go
case <- ctx.Done():
                    return
```

如此，则当cancel()被调用，或者达到超时时间，则case <- ctx.Done()被触发

5，如何执行多个goroutine，并且当一个goroutine执行完毕就返回

创建一个有缓冲的chan，然后每个goroutine在执行完毕时都往里面写数据，并且在main.main中进行<-chan操作，当其中一个goroutine执行完毕返回时，main.main中的<-chan就接收到数据了，解除阻塞

6，如何控制并发数量

通过有缓冲的chan，在goroutine中的逻辑执行前，先等待<-chan，执行结束后chan<-

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

7，生产者消费者模型

通过有缓冲的chan来实现，生产者往里面扔数据chan<-data，消费者从中消费数据<-chan