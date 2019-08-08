go读取channel的2种方法

go的gocontinue用法实现了轻量级的线程，可以被大量创建。线程之间用channal通信，以下说说基本用法。

​         channal的buf

c:=make(chan int ,3)

3就是设定的chanal的缓存量，缓存只意味着这个创建的channal可以同时存入3个数据，并不意味读channal时可以同时读出。

​        x:=<-c

​        读channal时仍然是以阻塞的方式一次一个地将数据读出。

​        当channal缓存中存在有效数据时，即使close掉channal，仍然可以读出buf里的数据。需要注意的是，从close掉的channal读数据时，channal是不阻塞的，当channal中的buf数据被读完时，仍然会以不阻塞的方式读出0值。

​      

​       channal的for range

​      c:=make(chan int )

​       for i:=range c {

}

​        for rangge的方式可以取出channal中的数据，它取数据的方式是**<u>*阻塞</u>***取数据，这和通常的方式是一致的，当channal中的数据为空时，它会阻塞等待数据。

如果在已经close掉channal的情况下，for range只会读完channal中的有效数据，然后接着往下执行，而不是向上面情况一样不断的读出0。

