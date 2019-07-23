go中defer和return的执行顺序

 

我将下面的代码放入了 double.go 文件内

`

```go
package main

//go:noinline
//go:nosplit

func test() (x int) {
    defer println("defer")
    return 200
}

func main() {
    println(test())
}
```

`

执行`go build -gcflags "-N -l" -o test double.go` 告诉编译器，不内联、不优化。生成可执行文件 test

反汇编来查看内容

![img](https:////upload-images.jianshu.io/upload_images/4759699-7f62a954f24637f1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)



注意：十进制的200 转化成16进制是 0xc8

通过上图的三个箭头可以看到。

- 先对返回值进行赋值：`MOVQ $0xc8, 0x30(SP)` 
- 执行 defer 语句: `CALL runtime.deferreturn(SB)` 
- 执行RET指令(函数返回): `RET` 

好的。执行顺序确定了。

`

```go
package main

//go:noinline
//go:nosplit

func test() (x int) {
    defer func() {
        x = 100
    }()
    return 200
}

func main() {
    println(test())
}
```

`

这道题的答案也就显而易见了。