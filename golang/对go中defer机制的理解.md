看下面一道题。

```
package main

import "fmt"

func calc(index string, a, b int) int {
	ret := a + b
	fmt.Println(index, a, b, ret)
	return ret
}

func main() {
	a := 3
	b := 2
	defer calc("1", a, calc("10", a, b))
	a = 0
	defer calc("2", a, calc("20", a, b))
	b = 1
}

// 输出：
// 10 3 2 5
// 20 0 2 2
// 2 0 2 2
// 1 3 5 8
```

![点击并拖拽以移动](data:image/gif;base64,R0lGODlhAQABAPABAP///wAAACH5BAEKAAAALAAAAAABAAEAAAICRAEAOw==)

有结果可以看出：

1，defer机制和栈类似，后进先出。所以calc("2", a, calc("20", a, b))先于calc("1", a, calc("10", a, b))执行；

2，但由于两个defer函数都含有函数型的参数，所以作为参数的函数在其位置展开执行，不需要遵守defer的规则。所以当执行到calc("1", a, calc("10", a, b))时，执行calc("10", a, b)求得一个值，当执行到calc("2", a, calc("20", a, b))时，执行calc("20", a, b)求得一个值；

3，由于defer机制的约束，在执行calc("2", a, calc("20", a, b))时，a为0，b为2；在执行calc("1", a, calc("10", a, b))是，a为3，b为2；