前言
反射在很多语言中都有其妙用。在计算机科学领域，反射是指一类应用，它们能够自描述和自控制。

本文将记录笔者对于Golang的反射的笔记。

以下知识点即将靠近： 
1.反射的简介 
2.为什么使用反射？ 
3.反射具体能做什么 
4.反射的一些小点

https://www.jianshu.com/p/53adb1e92710

正文
1.反射的简介
Golang提供了一种机制，在编译时不知道类型的情况下，可更新变量、运行时查看值、调用方法以及直接对他们的布局进行操作的机制，称为反射。

2.为什么使用反射？
打个比方，有时候我们需要一个函数可以处理各种类型的值。在不知道类型的情况下，你可能会这么写：

// 伪代码
switch value := value.(type) {
case string:
    // ...一些操作
case int:   
    // ...一些操作  
case cbsStruct: // 自定义的结构体  
    // ...一些操作

// ...
}

有没发现什么问题？ 
这边存在一个问题：类型很多，这个函数会写的非常长，而且还可能存在自定的类型，也就是说这个判断日后可能还要一直改，因为无法知道未知值到底属于什么类型。

无法透视一个未知类型的时候，以上代码其实不是很合理，这时候就需要有反射来帮忙你处理，反射使用TypeOf和ValueOf函数从接口中获取目标对象的信息，轻松完成目的。

3.反射具体能做什么？
1.获取变量内部信息

reflect提供了两种类型来进行访问接口变量的内容：

类型	作用
reflect.ValueOf()	获取输入参数接口中的数据的值，如果为空则返回0 <- 注意是0
reflect.TypeOf()	动态获取输入参数接口中的值的类型，如果为空则返回nil <- 注意是nil
上代码

package main

import (
    "fmt"
    "reflect"
)

func main() {
    var name string = "咖啡色的羊驼"

    // TypeOf会返回目标数据的类型，比如int/float/struct/指针等
    reflectType := reflect.TypeOf(name)
    
    // valueOf返回目标数据的的值，比如上文的"咖啡色的羊驼"
    reflectValue := reflect.ValueOf(name)
    
    fmt.Println("type: ", reflectType)
    fmt.Println("value: ", reflectValue)
}
输出：

type:  string
value:  咖啡色的羊驼

更深一层：在以上操作发生的时候，反射将“接口类型的变量”转为了“反射的接口类型的变量”，比如上文实际上返回的是reflect.Value和reflect.Type的接口对象。（可以根据ide跟踪一下相关函数返回类型便知）

2.struct的反射

package main

import (
    "fmt"
    "reflect"
)

type Student struct {
    Id   int
    Name string
}

func (s Student) Hello(){
    fmt.Println("我是一个学生")
}

func main() {
    s := Student{Id: 1, Name: "咖啡色的羊驼"}

    // 获取目标对象
    t := reflect.TypeOf(s)
    // .Name()可以获取去这个类型的名称
    fmt.Println("这个类型的名称是:", t.Name())
    
    // 获取目标对象的值类型
    v := reflect.ValueOf(s)
    // .NumField()来获取其包含的字段的总数
    for i := 0; i < t.NumField(); i++ {
        // 从0开始获取Student所包含的key
        key := t.Field(i)
    
        // 通过interface方法来获取key所对应的值
        value := v.Field(i).Interface()
    
        fmt.Printf("第%d个字段是：%s:%v = %v \n", i+1, key.Name, key.Type, value)
    }
    
    // 通过.NumMethod()来获取Student里头的方法
    for i:=0;i<t.NumMethod(); i++ {
        m := t.Method(i)
        fmt.Printf("第%d个方法是：%s:%v\n", i+1, m.Name, m.Type)
    }
}

输出：

这个类型的名称是: Student
第1个字段是：Id:int = 1 
第2个字段是：Name:string = 咖啡色的羊驼 
第1个方法是：Hello:func(main.Student)

3.匿名或嵌入字段的反射

package main

import (
    "reflect"
    "fmt"
)

type Student struct {
    Id   int
    Name string
}

type People struct {
    Student // 匿名字段
}

func main() {
    p := People{Student{Id: 1, Name: "咖啡色的羊驼"}}

    t := reflect.TypeOf(p)
    // 这里需要加一个#号，可以把struct的详情都给打印出来
    // 会发现有Anonymous:true，说明是匿名字段
    fmt.Printf("%#v\n", t.Field(0))
    
    // 取出这个学生的名字的详情打印出来
    fmt.Printf("%#v\n", t.FieldByIndex([]int{0, 1}))
    
    // 获取匿名字段的值的详情
    v := reflect.ValueOf(p)
    fmt.Printf("%#v\n", v.Field(0))
}

输出：

reflect.StructField{Name:"Student", PkgPath:"", Type:(*reflect.rtype)(0x10aade0), Tag:"", Offset:0x0, Index:[]int{0}, Anonymous:true}

reflect.StructField{Name:"Name", PkgPath:"", Type:(*reflect.rtype)(0x109f4e0), Tag:"", Offset:0x8, Index:[]int{1}, Anonymous:false}

main.Student{Id:1, Name:"咖啡色的羊驼"}

4.判断传入的类型是否是我们想要的类型

package main

import (
    "reflect"
    "fmt"
)

type Student struct {
    Id   int
    Name string
}

func main() {
    s := Student{Id: 1, Name: "咖啡色的羊驼"}
    t := reflect.TypeOf(s)

    // 通过.Kind()来判断对比的值是否是struct类型
    if k := t.Kind(); k == reflect.Struct {
        fmt.Println("bingo")
    }
    
    num := 1;
    numType := reflect.TypeOf(num)
    if k := numType.Kind(); k == reflect.Int {
        fmt.Println("bingo")
    }
}

输出：

bingo
bingo

5.通过反射修改内容

package main

import (
    "reflect"
    "fmt"
)

type Student struct {
    Id   int
    Name string
}

func main() {
    s := &Student{Id: 1, Name: "咖啡色的羊驼"}

    v := reflect.ValueOf(s)
    
    // 修改值必须是指针类型否则不可行
    if v.Kind() != reflect.Ptr {
        fmt.Println("不是指针类型，没法进行修改操作")
        return
    }
    
    // 获取指针所指向的元素
    v = v.Elem()
    
    // 获取目标key的Value的封装
    name := v.FieldByName("Name")
    
    if name.Kind() == reflect.String {
        name.SetString("小学生")
    }
    
    fmt.Printf("%#v \n", *s)


    // 如果是整型的话
    test := 888
    testV := reflect.ValueOf(&test)
    testV.Elem().SetInt(666)
    fmt.Println(test)
}


输出：

main.Student{Id:1, Name:"小学生"} 
666

6.通过反射调用方法

package main

import (
    "fmt"
    "reflect"
)

type Student struct {
    Id   int
    Name string
}

func (s Student) EchoName(name string){
    fmt.Println("我的名字是：", name)
}

func main() {
    s := Student{Id: 1, Name: "咖啡色的羊驼"}

    v := reflect.ValueOf(s)
    
    // 获取方法控制权
    // 官方解释：返回v的名为name的方法的已绑定（到v的持有值的）状态的函数形式的Value封装
    mv := v.MethodByName("EchoName")
    // 拼凑参数
    args := []reflect.Value{reflect.ValueOf("咖啡色的羊驼")}
    
    // 调用函数
    mv.Call(args)
}

输出：

我的名字是： 咖啡色的羊驼

4.反射的一些小点
1.使用反射时需要先确定要操作的值是否是期望的类型，是否是可以进行“赋值”操作的，否则reflect包将会毫不留情的产生一个panic。

2.反射主要与Golang的interface类型相关，只有interface类型才有反射一说。如果有兴趣可以看一下TypeOf和ValueOf，会发现其实传入参数的时候已经被转为接口类型了。

// 以下为截取的源代码
func TypeOf(i interface{}) Type {
    eface := *(*emptyInterface)(unsafe.Pointer(&i))
    return toType(eface.typ)
}

func ValueOf(i interface{}) Value {
    if i == nil {
        return Value{}
    }
    escapes(i)

​    return unpackEface(i)

}