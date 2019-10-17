安装mysql，版本是8.0

**1.下载mysql**

```shell
wget -i -c https://repo.mysql.com//mysql80-community-release-el7-3.noarch.rpm
```

  使用上面的命令就直接下载了安装用的Yum Repository，大概25KB的样子，然后就可以直接yum安装了。

```shell
yum -y install mysql80-community-release-el7-3.noarch.rpm
```

之后就开始安装MySQL服务器。

```shell
yum -y install mysql-community-server
```

这步可能会花些时间，安装完成后就会覆盖掉之前的mariadb。

 

### 2 MySQL数据库设置

启动mysql

```shell
systemctl start  mysqld.service
```

查看运行状态

```shell
systemctl status mysqld.service
```

![img](https://img2018.cnblogs.com/blog/1735716/201907/1735716-20190708232252988-726710168.png)

 

 

此时MySQL已经开始正常运行，不过要想进入MySQL还得先找出此时root用户的密码，通过如下命令可以在日志文件中找出密码：

 

```shell
grep "password" /var/log/mysqld.log
```

![img](https://img2018.cnblogs.com/blog/1735716/201907/1735716-20190708232336766-753330848.png)

 

进入数据库

```shell
mysql -uroot -p
```

  输入初始密码，此时不能做任何事情，因为MySQL默认必须修改密码之后才能操作数据库：

```shell
ALTER USER 'root'@'localhost' IDENTIFIED BY '新密码';
```

 这里有个问题，新密码设置的时候如果设置的过于简单会报错：

 原因是因为MySQL有密码设置的规范，具体是与validate_password_policy的值有关：

 

MySQL完整的初始密码规则查看

```shell
SHOW VARIABLES LIKE 'validate_password%';
```

初始情况下第一个的值是ON，validate_password_length是8。可以通过如下命令修改：

```shell
mysql> set global validate_password.policy=0;
mysql> set global validate_password.length=1;
```

 设置之后就是我上面查出来的那几个值了，此时密码就可以设置的很简单，例如1234之类的。到此数据库的密码设置就完成了。

  但此时还有一个问题，就是因为安装了Yum Repository，以后每次yum操作都会自动更新，需要把这个卸载掉：

```shell
yum -y remove mysql57-community-release-el7-10.noarch
```