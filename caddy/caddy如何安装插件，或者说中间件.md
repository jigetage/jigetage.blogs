caddy如何安装插件，或者说中间件

尝试了很多方法，包括GitHub找一些caddy manager等，都不是很好用

最后发现最易使用的是下载caddy的时候就把需要的插件加进去，这样下载的二进制caddy中已经含有了各种所需插件，可以直接使用

下载地址：

https://caddyserver.com/v1/download

选择好platform和plugins就可以了，plugins中就是所需要的插件

下载好了之后替换掉以前不含有插件的caddy，run起来就可以跑了

PS

提供包含http.cors,http.geoip,http.grpc,http.login,http.realip插件的caddy二进制文件

下载地址

链接:https://pan.baidu.com/s/1JihMdfRePQJvy2-t07EKRA  密码:rhz1

举例，如何使用http.realip插件

dev.ataotaoa.com {
  gzip
  log /var/log/caddy/access.log
  tls *******************************@qq.com {
    protocols tls1.0 tls1.2
  }
  realip {
    from 0.0.0.0/0
  }
}



