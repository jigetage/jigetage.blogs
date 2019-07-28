对于go.mod无法下载的包该如何处理？

解决办法一：

自己搭建goproxy服务器，然后在bash或者zsh中设置export GOPROXY=your_own_goproxy_url即可。

解决办法二：

例如:
你本地的包要引入 golang.org/x/net/html
但是被封了，那么你可以使用github上的镜像包 例如说是 github.com/golang/x/net/html

在你的项目的go.mod 中 加入 replace golang.org/x/net/html => github.com/golang/x/net/html
--------------------- 