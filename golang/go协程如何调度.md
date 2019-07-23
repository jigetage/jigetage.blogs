一、线程与协程的区别
1. 调度上的区别

   进程线程都是由操作系统进行调度， 有CPU时间片的概念，进行抢占式调度

   协程是用户态的轻量级线程，对内核透明，所以协程的调度与切换完全由用户控制

   正因为协程不由操作系统调度，才有：线程是操作系统调度的最小单位。

2. 切换开销的区别
  线程太重，资源占用太高，频繁创建销毁会带来严重的性能问题；
  协程切换远比线程小

  协程的好处：一个协程几乎就是一个普通的对象，因此可以放心阻塞，一旦阻塞那么让当前线程执行其他的协程（goroutine）

  

  二、goroutine和协程的区别

  goroutine是协程的go语言实现，相当于把别的语言的类库的功能内置到语言里。

  不同的是：
  Golang在runtime，系统调用等多方面对goroutine调度进行了封装和处理，即goroutine不完全是用户控制，一定程度上由go运行时（runtime）管理，好处：当某goroutine阻塞时，会让出CPU给其他goroutine。

三、goroutine的调度模型
1. 默认情况下，所有的goroutine都在同一个原生线程里跑，也就是只使用了一个CPU核。
    但是，通过runtime.GOMAXPROCS(4)设定，可以将goroutine调度到多个CPU上运行。
2. 在同一个原生线程里，若当前goroutine不发生阻塞，那么不会主动让出CPU给其他同一线程的goroutine的。
  在go程序启动时，会首先创建一个特殊的内核线程sysmom，负责监控和调度。



![20180706180251695](/Users/zhengxiao/Desktop/20180706180251695.png)



三类对象：
M代表线程
P代表处理器，每一个运行的M（线程）都必须绑定一个P（处理器）
G代表goroutine，每次使用go关键字的时候，都会创建一个G对象
图解：
当前有两个P，各自绑定了一个M，每个P上挂了一个本地goroutine队列，也有一个全局goroutine队列。
流程：
1. 每次使用go关键字声明时，一个G对象被创建并加入到本地G队列或者全局G队列。

2. 检查是否有空闲的P（处理器），若有那么创建一个M（若有正在sleep的M那么直接唤醒它）与其绑定，然后这个M循环执行goroutine任务。

3. G任务执行的顺序是，先从本地队列中找。但若某个M（线程）发现本地队列为空，那么会从全局队列中截取goroutine来执行（一次性转移（全局队列的G个数/P个数））。如果全局队列也空，那么会随机从别的P那里截取【一半】的goroutine过来（偷窃任务），若所有的P的队列都为空，那么该M（线程）就会陷入sleep。

4. 如果一个goroutine运行到一个“调度点”，上下文便从队列中取出一个goroutine，开始运行心得goroutine
  三种调度点：

  1、调用runtime.gosched函数。goroutine主动放弃CPU，该goroutine会被置为runnable状态，然后放入全局G队列，P继续执行下一个goroutine。

  主动行为

  使用场景：一般发生在执行长任务又想其他goroutine得到执行机会时调用。

  2、调用runtime.park函数。goroutine进入wait状态，除非对其调用runtime.ready函数，否则该goroutine永远不刽得到执行。而P继续执行下一个G任务。

  使用场景：读写channel。一般是在某个条件如果得不到满足就不能继续运行下去时调用，当条件满足后需要使用runtime.ready唤醒它，类似于Java里的await和notify
  3、慢系统调用。将该处理器P上设置为syscall状态，然后对应的线程M进入系统调用阻塞等待。sysmom监控线程会定期扫描所有的P（处理器），若发现一个P处于syscall状态，就讲=将M（线程）和P（处理器）分离，并再分配一个M与这个P绑定，从而继续执行P本地队列中的其他goroutine。当之前阻塞的M从系统调用返回后，将其执行的goroutine放入全局G队列中，该M去sleep。如下图。



![20180706180313415](/Users/zhengxiao/Desktop/20180706180313415.png)



sysmom线程的执行过程：
1. 记录每一个P（处理器）执行的goroutine数schedtick，每执行一个goroutine后schedtick递增
2. 如果检查到schedtick一直没有递增，就说明这个P一直在执行同一个goroutine，如果超过一定的时间阈值（10ms），就在这个goroutine任务的栈信息里面加一个标记
3. 然后这个goroutine任务在执行的时候，如果遇到非内联函数调用，就会检查一次这个标记，然后中断自己，把自己加到队列末尾，执行下一个G（即所谓的调度点3）
4. 如果没有遇到非内联函数调用（内联函数：执行权不转交给被调用函数，而是将函数的代码粘贴在调用处），那么就会一直执行这个goroutine任务，直到他自己结束。

