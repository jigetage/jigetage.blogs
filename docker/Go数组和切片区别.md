Go数组和切片区别

当作为参数传给函数时，如果在函数内部被修改了，那么数组是临时修改，即出了函数后，原来的数据还是原来的数据，而切片是永久修改，即出了函数后，原来的数据已经变了，变成了修改后的数据。

例子：

``

```go
package main

import "fmt"

func f0(a [3]int64)  {
   a[0] += 1
   fmt.Println(a[0])
   a[1] += 1
   fmt.Println(a[1])
   a[2] += 1
   fmt.Println(a[2])
}

func f1(a *[3]int64)  {
   a[0] += 1
   fmt.Println(a[0])
   a[1] += 1
   fmt.Println(a[1])
   a[2] += 1
   fmt.Println(a[2])
}

func f2(a []int64)  {
   a[0] += 1
   fmt.Println(a[0])
   a[1] += 1
   fmt.Println(a[1])
   a[2] += 1
   fmt.Println(a[2])
}

func main()  {
   vvd := [3]int64{1, 2, 3}
   fmt.Println(vvd[0])
   fmt.Println(vvd[1])
   fmt.Println(vvd[2])

   f0(vvd)

   fmt.Println(vvd[0])
   fmt.Println(vvd[1])
   fmt.Println(vvd[2])

   fmt.Println("++++++")

   v := [3]int64{1, 2, 3}
   fmt.Println(v[0])
   fmt.Println(v[1])
   fmt.Println(v[2])

   f1(&v)

   fmt.Println(v[0])
   fmt.Println(v[1])
   fmt.Println(v[2])

   fmt.Println("******")

   vv := []int64{1, 2, 3}
   fmt.Println(vv[0])
   fmt.Println(vv[1])
   fmt.Println(vv[2])

   f2(vv)

   fmt.Println(vv[0])
   fmt.Println(vv[1])
   fmt.Println(vv[2])
}
```

Output:

1
2
3
2
3
4
1
2
3
++++++
1
2
3
2
3
4
2
3
4
******
1
2
3
2
3
4
2
3
4