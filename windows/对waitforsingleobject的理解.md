当调用waitforsingleobject被阻塞时，说明在调用的时候所监视的内核对象是non-signaled状态。然后，开始阻塞等待，突然，发生了某种情况，导致监视的内核对象变成了signaled状态，这时waitforsingleobject函数解除阻塞，开始运行，运行完了，函数返回。在返回后，如果所监视的内核对象是自动置位的，那么内核对象变为non-signaled状态，否则继续保持signaled状态。

总结为：non-singaled -> singaled -> non-singaled

