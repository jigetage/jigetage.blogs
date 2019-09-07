## 最佳实践

1. context.Background 只应用在最高等级，作为所有派生 context 的根。
2. context.TODO 应用在不确定要使用什么的地方，或者当前函数以后会更新以便使用 context。
3. context 取消是建议性的，这些函数可能需要一些时间来清理和退出。
4. context.Value 应该很少使用，它不应该被用来传递可选参数。这使得 API 隐式的并且可以引起错误。取而代之的是，这些值应该作为参数传递。
5. 不要将 context 存储在结构中，在函数中显式传递它们，最好是作为第一个参数。
6. 永远不要传递不存在的 context 。相反，如果您不确定使用什么，使用一个 ToDo context。
7. Context 结构没有取消方法，因为只有派生 context 的函数才应该取消 context。



code

```go
//Function that does slow processing with a context
//Note that context is the first argument
func sleepRandomContext(ctx context.Context, ch chan bool) {
    //Cleanup tasks
    //There are no contexts being created here
    //Hence, no canceling needed
    defer func() {
        fmt.Println("sleepRandomContext complete")
        ch <- true
    }()
    //Make a channel
    sleeptimeChan := make(chan int)
    //Start slow processing in a goroutine
    //Send a channel for communication
    go sleepRandom("sleepRandomContext", sleeptimeChan)
    //Use a select statement to exit out if context expires
    select {
    case <-ctx.Done():
        //If context expires, this case is selected
        //Free up resources that may no longer be needed because of aborting the work
        //Signal all the goroutines that should stop work (use channels)
        //Usually, you would send something on channel,
        //wait for goroutines to exit and then return
        //Or, use wait groups instead of channels for synchronization
        fmt.Println("Time to return")
    case sleeptime := <-sleeptimeChan:
        //This case is selected when processing finishes before the context is cancelled
        fmt.Println("Slept for ", sleeptime, "ms")
    }
}
```