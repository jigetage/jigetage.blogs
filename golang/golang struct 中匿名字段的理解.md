golang struct 中匿名字段的理解

Go语言不支持传统面向对象中的继承特性，而是以自己特有的组合方式支持了方法的继承。Go语言中，通过在结构体内置匿名的成员来实现继承：

```go
import "image/color"

type Point struct{ X, Y float64 }

type ColoredPoint struct {
    Point
    Color color.RGBA
}
```

虽然我们可以将`ColoredPoint`定义为一个有三个字段的扁平结构的结构体，但是我们这里将`Point`嵌入到`ColoredPoint`来提供`X`和`Y`这两个字段。

```go
var cp ColoredPoint
cp.X = 1
fmt.Println(cp.Point.X) // "1"
cp.Point.Y = 2
fmt.Println(cp.Y)       // "2"
```

通过嵌入匿名的成员，我们不仅可以继承匿名成员的内部成员，而且可以继承匿名成员类型所对应的方法。我们一般会将Point看作基类，把ColoredPoint看作是它的继承类或子类。不过这种方式继承的方法并不能实现C++中虚函数的多态特性。所有继承来的方法的接收者参数依然是那个匿名成员本身，而不是当前的变量。

```go
type Cache struct {
    m map[string]string
    sync.Mutex
}

func (p *Cache) Lookup(key string) string {
    p.Lock()
    defer p.Unlock()

    return p.m[key]
}
```

`Cache`结构体类型通过嵌入一个匿名的`sync.Mutex`来继承它的`Lock`和`Unlock`方法. 但是在调用`p.Lock()`和`p.Unlock()`时, `p`并不是`Lock`和`Unlock`方法的真正接收者, 而是会将它们展开为`p.Mutex.Lock()`和`p.Mutex.Unlock()`调用. 这种展开是编译期完成的, 并没有运行时代价.

在传统的面向对象语言(eg.C++或Java)的继承中，子类的方法是在运行时动态绑定到对象的，因此基类实现的某些方法看到的`this`可能不是基类类型对应的对象，这个特性会导致基类方法运行的不确定性。而在Go语言通过嵌入匿名的成员来“继承”的基类方法，`this`就是实现该方法的类型的对象，Go语言中方法是编译时静态绑定的。如果需要虚函数的多态特性，我们需要借助Go语言接口来实现。

上面的话总结下来就是：

1，匿名字段引入了基类

2，可以理解为将基类中的字段扁平化处理，和struct中有名字段一个层级访问

3，匿名字段的方法绑定的对象还是匿名字段，而不是外部struct

