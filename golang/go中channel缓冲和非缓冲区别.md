分析下面一道题目，再次感受channel的缓冲和非缓冲的区别

```go
// 错误写法
func (set *threadSafeSet) Iter() <-chan interface{} {
	ch := make(chan interface{})
	go func() {
		set.RLock()

		for elem := range set.s {
			ch <- elem
		}

		close(ch)
		set.RUnlock()

	}()
	return ch
}

// 正确写法
func (set *threadSafeSet) Iter() <-chan interface{} {
    // ch := make(chan interface{}) // 解除注释看看！
    ch := make(chan interface{},len(set.s))
    go func() {
        set.RLock()

        for elem,value := range set.s {
            ch <- elem
            println("Iter:",elem,value)
        }

        close(ch)
        set.RUnlock()

    }()
    return ch
}

```

对于错误的那段代码，由于channel未指定容量，所以是无缓冲的，那么在调用Iter的时候，第一次写入后便会阻塞，无法实现遍历整个slice。正确的那段代码，在定义channel的时候，指定了容量，所以是有缓冲的，并且指定的容量正好是slice的长度，所以可以实现遍历整个slice。