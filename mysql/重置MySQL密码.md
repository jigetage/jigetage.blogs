重置MySQL密码

- 编辑 my.cnf　

  

- 重启 MySQL 服务

```shell
[root@VM_0_7_centos mysql]# service mysqld restart
```

- 进入MySQL

```shell
[root@VM_0_7_centos mysql]# mysql -uroot
```

- 更新密码

1. flush privileges;
2. alter user 'root'@'localhost' identified with mysql_native_password by '123456';
3. flush privileges;
4. exit;

- 编辑 my.cnf  注释掉 skip-grant-tables
- 重启MySQL服务

```shell
[root@VM_0_7_centos mysql]# service mysqld restart
```

- 进入MySQL 输入密码

```shell
mysql -uroot -p
```

- 结束