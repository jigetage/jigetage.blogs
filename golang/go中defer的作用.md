go中defer的作用。

1，在函数运行结束时，进行清理操作。比如：

关闭文件流 

```
// open a file  
defer file.Close()
```

解锁一个加锁的资源 

```
mu.Lock()  
defer mu.Unlock() 
```

打印最终报告

```
printHeader()  
defer printFooter()
```

关闭数据库链接

```
// open a database connection  
defer disconnectFromDB()
```

2，对函数return的返回值进行操作

```
package main

import (
	"io"
	"log"
)

func func1(s string) (n int, err error) {
	defer func() {
		log.Printf("func1(%q) = %d, %v", s, n, err)
	}()
	return 7, io.EOF
}

func main() {
	func1("Go")
}
```

输出：

```go
Output: 2011/10/04 10:46:11 func1("Go") = 7, EOF
```

defer是可以访问函数返回值7和EOF的。