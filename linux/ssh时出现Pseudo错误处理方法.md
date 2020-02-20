ssh时出现Pseudo错误处理方法

添加-Tq参数

比如：

sshpass -p yskj2407 ssh -Tq -L 0.0.0.0:12347:10.66.175.9:3306 lambda@123.206.220.183

