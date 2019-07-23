对于go语言而言，通过对象和指针这两种方式调用method都可以，因为Go知道receiver是指针，他自动帮你转了。

也就是说：

> 如果一个method的receiver是*T,你可以在一个T类型的实例变量V上面调用这个method，而不需要&V去调用这个method

类似的

> 如果一个method的receiver是T，你可以在一个*T类型的变量P上面调用这个method，而不需要 *P去调用这个method

