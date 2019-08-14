彻底弄懂go mod

第一篇文章

golang 终于出官方版本管理机制,名为 go modules

## 初体验

使用前:

```bash
# 先升级 golang 到 1.11 版本,然后
export GO111MODULE=on
```

在项目github.com/humboldt-xie/test-mod下，通过go mod init

```bash
go mod init
```

然后会在当前项目目录下出现 go.mod 文件，内容为:

```go
module github.com/humboldt-xie/test-mod
```

编辑代码 main.go

```go
package main

import (
	"fmt"
	"github.com/humboldt-xie/hlib/vmap"
)

func main() {
	vmap := vmap.NewVMap()
	fmt.Println("vim-go", vmap)
}
```

引用一个包

然后，直接运行go build ./

```bash
➜  go build ./
go: finding github.com/humboldt-xie/hlib/vmap latest
go: finding github.com/humboldt-xie/hlib latest
```

这时，会自动加载依赖包,并编译通过

这时，多了一个文件go.sum，并且go.mod 也被修改了

go.mod 则是描述直接依赖包(相当于glide.yaml)

go.sum 则是描述依赖树锁定(相当于glide.lock)

查看go.mod 文件内容，这时变为了

```bash
module github.com/humboldt-xie/test-mod

require github.com/humboldt-xie/hlib v0.0.0-20180903073735-6738efa10c3a
```

go.sum 内容为

```bash
github.com/humboldt-xie/hlib v0.0.0-20180903073735-6738efa10c3a h1:GgiozN6lA73wd7mQbJUdiEzW9CoBJM1Ey8A3G8RbsCo=
github.com/humboldt-xie/hlib v0.0.0-20180903073735-6738efa10c3a/go.mod h1:Zdztzq6jaF9E1ZbVz6g5Kf/Thx4/guqFB3gxAkcNkGs=
github.com/timtadh/data-structures v0.5.2 h1:yGrL+5Tf5mwRDPGnKwLeLOvfmoDsuKiMRmpETgGq+4w=
github.com/timtadh/data-structures v0.5.2/go.mod h1:9R4XODhJ8JdWFEI8P/HJKqxuJctfBQw6fDibMQny2oU=
```

go mod 还支持的命令:

```bash
	download    download modules to local cache
	edit        edit go.mod from tools or scripts
	graph       print module requirement graph
	init        initialize new module in current directory
	tidy        add missing and remove unused modules
	vendor      make vendored copy of dependencies
	verify      verify dependencies have expected content
	why         explain why packages or modules are needed
```

## 存储

go mod 下载的库，都存储在

$GOPATH/pkg/mod/cache 下

## 总结

使用go mod 总共分为两步:

```bash
go mod init

go build ./
```



第二篇文章

### 一、使用方式

1. 首先完全开启 `go modules`
   三种模式: `on/auto/off`，默认为 auto: `$GOPATH`下的项目不开启，非`$GOPATH`下的开启

```bash
$ export GO111MODULE=on
```

1. 在项目里初始化 `go.mod`

```bash
$ go mod init my-project-package-name
```

1. 按需替换被墙的 module 地址

```bash
$ vi go.mod
replace (
    cloud.google.com/go => github.com/GoogleCloudPlatform/google-cloud-go latest
    golang.org/x/crypto => github.com/golang/crypto latest
    golang.org/x/net => github.com/golang/net latest
    golang.org/x/sync => github.com/golang/sync latest
    golang.org/x/sys => github.com/golang/sys latest
    golang.org/x/text => github.com/golang/text latest
    google.golang.org/appengine => github.com/golang/appengine latest
)
```

1. 自动下载依赖的 module（同时删除掉无用的）

```bash
$ go mod tidy -v
```

- `-v`啰嗦模式
- 完成后，生成`go.mod`的`require`部分，和`go.sum`文件
- 可以在`.gitignore`中忽略掉`go.sum`，只需要保留`go.mod`。
- 依赖的 module 被文件下载到`$GOPATH/pkg/cache/`和`$GOPATH/pkg/mod/`文件夹下了

### 二、问题修复

通过开启 `useLanguageServer`，修复 vscode 代码提示、跳转到定义等问题

```json
$ go get github.com/saibing/bingo
{
  "go.alternateTools": {
    "go-langserver": "bingo"
  },
  "go.languageServerExperimentalFeatures": {
    "format": true,
    "autoComplete": true
  },
  "go.useLanguageServer": true
}
```

[参考文档](https://github.com/Microsoft/vscode-go/wiki/Go-modules-support-in-Visual-Studio-Code)

### 三、切换回 `Vendor`模式

目前 go mod 各方面支持还不够完善，如果想切换回 `vendor`模式，可以通过下面两种方式

```bash
# 临时切换
$ go mod vendor
$ go run -mod=vendor main.go

# 或设置成全局环境变量
# export GOFLAGS=-mod=vendor
$ go run main.go
```

------

### 四、后续

用了一段时间 go mod 后，发现 vscode 目前的支持非常差，代码提示方面存在各种问题，提示速度也慢了很多，难以忍受，所以选择暂时放弃使用，待完善后再来。
目前切换成 `dep`方式管理依赖包

```bash
# 初始化
$ dep init

# 整理依赖包
$ dep ensure

# 更新依赖版本
$ dep ensure -update
```