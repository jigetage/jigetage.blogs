go中传递指针和传递值的区别

在 `Go` 中函数传递参数有传值和传指针两种类型，本文将从细节之处剖析两者的不同。

先看一个`demo`：

`package main

import (
  "encoding/json"
  "fmt"
)

type Girl struct {
  Name       string `json:"name"`
  DressColor string `json:"dress_color"`
}

func (g Girl) SetColor(color string) {
  g.DressColor = color
}
func (g Girl) GetJson() string {
  data, _ := json.Marshal(&g)
  return string(data)
}
func main() {
  g := Girl{Name: "yueyue"}
  g.SetColor("white")
  fmt.Println(g.GetJson())
}`

将打印出什么结果呢？

将输出：

> {"name":"yueyue","dress_color":""}

咦，为啥颜色没有设置成功？

仔细思考，原来就是今天要分析的 `Golang` 中关于函数传值与传指针的区别没搞清楚。首先，我们看到 `SetColor` 和 `GetJson` 函数都是值传递，所以实际在 `main` 中调用 `g.SetColor` 的时候，是拷贝了一份副本给函数 `SetColor`，然后在函数内对副本进行了 `color` 的设置；但实际上此时原来的 `g` 对象却依然只有 `name` 属性，所以输出了以上结果。

我们来打印一下传值前后对应的 `g` 是不是同一个对象就知道了：

`package main

import (
  "encoding/json"
  "fmt"
)

type Girl struct {
  Name       string `json:"name"`
  DressColor string `json:"dress_color"`
}

func (g Girl) SetColor(color string) {
  fmt.Printf("g1: %p\n", &g)
  g.DressColor = color
}
func (g Girl) GetJson() string {
  data, _ := json.Marshal(&g)
  return string(data)
}
func main() {
  g := Girl{Name: "yueyue"}
  fmt.Printf("g0: %p\n", &g)

  g.SetColor("white")
  fmt.Println(g.GetJson())
}`

输出结果：

> g0: 0xc42000a060
>
> g1: 0xc42000a080
>
> {"name":"yueyue","dress_color":""}

发现确实 `g0` 与 `g1` 对象的内存地址是不同的，说明是两个不同的对象。接下来，我们看一下传指针的情况：

`package main

import (
  "encoding/json"
  "fmt"
)

type Girl struct {
  Name       string `json:"name"`
  DressColor string `json:"dress_color"`
}

func (g *Girl) SetColor(color string) {
  fmt.Printf("g1: %p\n", g)
  g.DressColor = color
}
func (g *Girl) GetJson() string {
  data, _ := json.Marshal(&g)
  return string(data)
}
func main() {
  g := &Girl{Name: "yueyue"}
  fmt.Printf("g0: %p\n", g)

  g.SetColor("white")
  fmt.Println(g.GetJson())
}`

输出结果：

> g0: 0xc42000a060
>
> g1: 0xc42000a060
>
> {"name":"yueyue","dress_color":"white"}

可以发现，函数传指针前后是对象的内存地址是相同的，所以 `SetColor` 将会生效。我们还可以看下实际上传递的依然是值拷贝，只不过是指针拷贝了一份副本，两个指针指向相同的 `g` 对象而已，代码如下：

package main

import (
  "encoding/json"
  "fmt"
)

type Girl struct {
  Name       string `json:"name"`
  DressColor string `json:"dress_color"`
}

func (g *Girl) SetColor(color string) {
  fmt.Printf("g1: %p\n", &g) // 取指针的地址
  g.DressColor = color
}
func (g *Girl) GetJson() string {
  data, _ := json.Marshal(&g)
  return string(data)
}
func main() {
  g := &Girl{Name: "yueyue"}
  fmt.Printf("g0: %p\n", &g) // 取指针的地址

  g.SetColor("white")
  fmt.Println(g.GetJson())
}

输出结果：

> g0: 0xc42000c028
>
> g1: 0xc42000c038
>
> {"name":"yueyue","dress_color":"white"}

所以，在 `Golang` 中所有函数参数传递都是值拷贝，传指针只是拷贝了一份指针副本，同时指向原对象。

**小结**：在函数传参过程中，需要合理使用传值、传指针。一般情况下，需要改变原始对象值、传递大的结构体，传指针是最合适的，因为传一个内存地址的开销很小。反之，如果变量不可变更、`map` 或 `slice` 应该选择传值方式。