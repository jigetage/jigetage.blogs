ssh -L/-R/-D参数说明

假设说HostA和HostB是互通的。

## ssh -L

此时配置的是本地端口转发，比如说：

```shell
HostB$ ssh -L 8888 UserC@HostC
```

在 HostA 上可以使用 HostB:8888 就像使用 HostC:22 一样，注意此时是在HostB上操作的。

```shell
HostA$ ssh -L 8888:HostC:22 UserB@HostB
```

在 HostA 上可以使用 localhost:8888 就像使用 HostC:22 一样，注意此时是在HostA 上操作的

## ssh -R

此时配置的是远端端口转发。

```shell
HostC$ ssh -R 8888:localhost:22 UserB@HostB
```

在 HostA 上可以使用 HostB:8888 就像使用 HostC:22 一样，注意此时是在HostC上操作的。

```shell
HostC$ ssh -R 8888:HostD:22 UserB@HostB1
```

在 HostA 上可以使用 HostB:8888 就像使用 HostD:22 一样，注意此时是在HostC上操作的。

## ssh -D

```shell
HostA$ ssh -D 8080
```

此时 localhost:8080 就是本地的 Socket 代理服务器。效果，即浏览器配置了 本机IP:8080 Socket的代理后，就可以像在本机上一样访问网站资源了。

```
HostA$ ssh -D 8080 UserB:HostB
或写全
HostA$ ssh -D localhost:8080 -p 22 UserB:HostB123
```

此时 localhost:8080 就是HostB上的 Socket 代理服务器。效果，即浏览器配置了 本机IP:8080 Socket5 的代理后，就可以像在HostB上一样访问网站资源了。