解决Host is not allowed to connect to this MySQL server问题



1. 在装有MySQL的机器上登录MySQL mysql -u root -p密码

2. 执行`use mysql;`

3. 执行`update user set host = '%' where user = 'root';`这一句执行完可能会报错，不用管它。

4. 执行`FLUSH PRIVILEGES;`

   

