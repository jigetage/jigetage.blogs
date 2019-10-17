如何查看Linux各种版本

预先知识：Linux版本分为内核版本和发行版本

查看内核版本：uname -a

Linux master 3.10.0-957.27.2.el7.x86_64 #1 SMP Mon Jul 29 17:46:05 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux

查看完整内核版本：cat /proc/version

Linux version 3.10.0-957.27.2.el7.x86_64 (mockbuild@kbuilder.bsys.centos.org) (gcc version 4.8.5 20150623 (Red Hat 4.8.5-36) (GCC) ) #1 SMP Mon Jul 29 17:46:05 UTC 2019

查看发行版本：cat /etc/issue（针对ubuntu） or cat /etc/redhat-release（针对centos）

CentOS Linux release 7.6.1810 (Core)