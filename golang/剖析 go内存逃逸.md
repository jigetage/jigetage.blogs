# 什么是内存逃逸

分配在栈上的内存被自动分配到堆上

# 何时触发内存逃逸

首先明确一点：内存逃逸由编译器自动触发，无须手工编写代码

逃逸分析在编译环节进行

1，返回局部指针变量引起逃逸

2，栈空间不足引起逃逸

3，动态类型引起逃逸

4，闭包引起逃逸

# 逃逸总结

栈上分配内存比在堆中分配内存有更高的效率

栈上分配的内存不需要GC处理

堆上分配的内存使用完毕会交给GC处理

逃逸分析目的是决定内分配地址是栈还是堆

逃逸分析在编译阶段完成