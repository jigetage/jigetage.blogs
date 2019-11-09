linux定时任务crontab

crontab是一个用来设置、删除或显示供守护进程cron执行的定时任务的命令。每一个用户都可以拥有属于自己的定时任务，定时任务文件默认以用户名命名，并放在/var/spool/cron目录，该目录普通用户无访问权限。

　　可以通过cron.allow 和 cron.deny文件管理用户使用cron服务的权限。如果cron.allow存在，用户必须列在其中才会被允许使用cron服务；如果cron.deny存在，被列在其中的用户禁止使用cron服务；如果两者都不存在，只有超级用户才能使用cron服务。在CentOS中，这两个文件放在/etc中，默认只存在cron.deny文件，且为空。这就表示在CentOS中，默认所有的用户均有cron的使用权限。

**1. 命令格式**

　　crontab [-u user] file
　　crontab [-u user] [-l | -r | -e] [-i] [-s]
　　crontab -n [ hostname ]
　　crontab -c

**2. 命令功能**

　　为个人用户维护定时任务文件

**3. 命令选项**

　　-u user

　　　　编辑某个用户的cron，只用root可以使用此参数为别的用户设置cron服务。也可用于给用户指定crontab文件。

　　-l

　　　　列出当前用户的crontab。

　　-r

　　　　删除当前用户的crontab。

　　-e

　　　　编辑当前用户的crontab，默认使用vi，或者是由 VISUAL或EDITOR环境变量指定的编辑器。

　　-i

　　　　与-r一起使用，用来询问用户是否直接删除crontab。

**4. 实例**

　　实例1：列出当前用户的定时任务

```shell
[22:30:17][dodmaster@mha3 ~]$ crontab -l
0 1 * * 1 $HOME/.DailyShell/hislogclear.sh 
0 * * * * $HOME/.DailyShell/mvcdr2bak.sh $HOME/data/message/voice
20 * * * * $HOME/.DailyShell/mvcdr2bak.sh $HOME/data/message/data/
40 * * * * $HOME/.DailyShell/mvcdr2bak.sh $HOME/data/message/sms/
```

　　实例2：root用户编辑用户martin的定时任务

```shell
[22:30:17][root@mha3 ~]# crontab -u martin -e
```

　　实例3：为用户martin指定定时任务文件

```shell
[22:30:17][root@mha3 ~]# crontab -u martin martincron
```

**5. cron文件语法**

 　　首先，我们打开/etc/crontab文件，我们将会看到如下内容：

```shell
[20:40:30][dodmaster@mha3 etc]$ cat /etc/crontab 
SHELL=/bin/bash
PATH=/sbin:/bin:/usr/sbin:/usr/bin
MAILTO=root
HOME=/

# For details see man 4 crontabs

# Example of job definition:
# .---------------- minute (0 - 59)
# |  .------------- hour (0 - 23)
# |  |  .---------- day of month (1 - 31)
# |  |  |  .------- month (1 - 12) OR jan,feb,mar,apr ...
# |  |  |  |  .---- day of week (0 - 6) (Sunday=0 or 7) OR sun,mon,tue,wed,thu,fri,sat
# |  |  |  |  |
# *  *  *  *  * user-name command to be executed
```

　　该文件是存放系统执行的定时计划，其中前四行用于设置cron服务运行的环境变量：SHELL、PATH、HOME分别指定了cron服务运行时的SHELL环境变量、PATH环境变量和HOME环境变量，这里不再赘述。MAILTO则表示cron运行的任务输出以电子邮件的形式发送到指定用户，如果该变量的值为空，则不会发送邮件。余下几行的内容说明了系统定时任务的具体书写格式。

| minute       | 分钟，取值范围0-59                                           |
| ------------ | ------------------------------------------------------------ |
| hour         | 小时，取值范围0-23                                           |
| day of month | 日，取值范围1-31                                             |
| month        | 月，取值范围1-12，或者使用英文缩写jan,feb,mar,apr ...        |
| day of week  | 星期，取值范围0-6，0或7表示星期日，或者使用英文缩写sun,mon,tue,wed,thu,fri,sat |
| user-name    | 执行该定时任务的用户                                         |
| command      | 具体执行的命令，可以是一个简单的命令，也可以是一个脚本，或者是一个目录。如果是一个目录，则表示执行目录中的所有脚本，目录的前面必须加上run-parts。 |

　　1）星号(*)表示取值范围内的所有值。例如，*在hour的位置，表示每小时执行一次。

　　2）连字符(-)表示一个范围。例如，8-12表示8、9、10、11、12。

　　3）逗号(,)表示分割指定的数值。例如：3,5-7,9表示3,5,6,7,9。

　　4）正斜杠(/)表示步进值。例如，分钟的位置为*/5，表示每五分钟执行一次。

　　除了root用户以外的所有用户定义的crontab计划任务都存放在/var/spool/cron目录，通过**crontab -e**命令编辑，格式与/etc/crontab相同，可以不用指定user-name。

　　cron服务会每分钟检查一次/etc/crontab、/etc/cron.d/和/var/spool/cron/中的所有文件，并依此执行。

**6. 实例**

　　实例1：每周末的凌晨一点钟执行一次系统的raid-check

```shell
0 1 * * Sun root /usr/sbin/raid-check
```

　　实例2：每天的4:02am分root用户执行/etc/cron.daily目录下的所有脚本，run-parts参数表示执行后面目录中的所有脚本。

```shell
02 4 * * * root run-parts /etc/cron.daily
```

　　实例3：每天的8-18每四小时，dodmaster用户执行hislogclear.sh脚本，即8:44,12:44,16:44,20:44四个时间点，会执行该脚本。

```shell
[21:57:56][dodmaster@mha3 ~]$ crontab -l
44 8-20/4 * * * $HOME/.DailyShell/hislogclear.sh 
```

　　实例4：每五分钟执行一次

```shell
*/5 * * * * echo "every five minute to do it" >> time.txt
```

　　实例5：每个工作日下午八点执行

```shell
0 20 * * 1-5 echo "every workday 20:00 to do it" >> time.txt
```

　　实例6：每分钟的30秒时执行，因为cron服务是每分钟被唤醒一次，所以要执行精确到秒的任务需要借助sleep命令。

```shell
* * * * * sleep 30； echo "every minute 30 second to do it" >> time.txt
```

　　实例7：每20秒执行一次，即表示在每分钟的0秒、20秒、40秒各执行一次（也可以是其他的步长是20秒的值，例如：5s、25s、45s）

```shell
* * * * * echo "every 20 second to do it" >> time.txt
* * * * * sleep 20； echo "every 20 second to do it" >> time.txt
* * * * * sleep 40； echo "every 20 second to do it" >> time.txt
```