Docker CMD & ENTRYPOINT

昨天用Dockerfile来启动mongodb的集群，启动参数--replSet死活没执行，最后就决定研究一哈cmd和entrypoint。但是上网看了一些资料个人觉得讲的不好，还是没有说出根本的东西，决定自己研究并且整理一哈。

首先上docker官网：https://docs.docker.com/engine/reference/builder/#cmd

感觉官网关于这两个命令讲的简直不要太清楚。



cmd：

这个命令是用来做什么的？下面是官网的答案：

The main purpose of a CMD is to provide defaults for an executing container. These defaults can include an executable, or they can omit the executable, in which case you must specify an ENTRYPOINT instruction as well.
意思是，cmd给出的是一个容器的默认的可执行体。也就是容器启动以后，默认的执行的命令。重点就是这个“默认”。意味着，如果docker run没有指定任何的执行命令或者dockerfile里面也没有entrypoint，那么，就会使用cmd指定的默认的执行命令执行。同时也从侧面说明了entrypoint的含义，它才是真正的容器启动以后要执行命令。
所以这句话就给出了cmd命令的一个角色定位，它主要作用是默认的容器启动执行命令。（注意不是“全部”作用）

这也是为什么大多数网上博客论坛说的“cmd会被覆盖”，其实为什么会覆盖？因为cmd的角色定位就是默认，如果你不额外指定，那么就执行cmd的命令，否则呢？只要你指定了，那么就不会执行cmd，也就是cmd会被覆盖。

明白了cmd命令的主要用途。下面就看看具体用法。

总共有三种用法：

The CMD instruction has three forms:

CMD ["executable","param1","param2"] (exec form, this is the preferred form)
CMD ["param1","param2"] (as default parameters to ENTRYPOINT)
CMD command param1 param2 (shell form)

因为还没有讲entrypoint，所以先不看用法2。
用法3：shell form，即没有中括号的形式。那么命令command默认是在“/bin/sh -c”下执行的。比如下面的dockerfile：

FROM centos

CMD echo "hello cmd!"
运行：

用法1：带有中括号的形式。这时，命令没有再任何shell终端环境下，如果我们要执行shell，必须把shell加入到中括号的参数中。这种用法就像一个c语言的exec函数，意思是我们要执行一个进程。如果采用非shell的方法，那么上面的例子要修改为：

FROM centos

CMD ["/bin/bash", "-c", "echo 'hello cmd!'"]
需要注意，采用中括号形式，那么第一个参数必须是命令的全路径才行。而且，一个dockerfile至多只能有一个cmd，如果有多个，只有最后一个生效。
官网推荐采用这种方法。

当然，以上都是体现了cmd的“默认”行为。如果我们在run时指定了命令或者有entrypoint，那么cmd就会被覆盖。仍然是上面的image。run命令变了：





可以看到，最终容器里面执行的是run命令后面的命令，而不是cmd里面定义的。



接下来再看entrypoint：

An ENTRYPOINT allows you to configure a container that will run as an executable.
也就是说entrypoint才是正统地用于定义容器启动以后的执行体的，其实我们从名字也可以理解，这个是容器的“入口”。
有两种用法：

ENTRYPOINT has two forms:

ENTRYPOINT ["executable", "param1", "param2"] (exec form, preferred)
ENTRYPOINT command param1 param2 (shell form)

命令行和shell。
先看命令行模式，也就是带中括号的。和cmd的中括号形式是一致的，但是这里貌似是在shell的环境下执行的，与cmd有区别。如果run命令后面有东西，那么后面的全部都会作为entrypoint的参数。如果run后面没有额外的东西，但是cmd有，那么cmd的全部内容会作为entrypoint的参数，这同时是cmd的第二种用法。这也是网上说的entrypoint不会被覆盖。当然如果要在run里面覆盖，也是有办法的，使用--entrypoint即可。

下面看几个例子。

dockerfile为：

FROM centos

CMD ["p in cmd"]
ENTRYPOINT ["echo"]
如果run不带参数：


如果run带参数：



而且，确实entrypoint的中括号形式下，command是在shell环境下运行的，否则这里的echo是无法被执行的。

第二种是shell模式的。在这种模式下，任何run和cmd的参数都无法被传入到entrypoint里。官网推荐第一种用法。

FROM centos

CMD ["p in cmd"]
ENTRYPOINT echo


cmd的参数没有被打印。



总结下一般该怎么使用：一般还是会用entrypoint的中括号形式作为docker 容器启动以后的默认执行命令，里面放的是不变的部分，可变部分比如命令参数可以使用cmd的形式提供默认版本，也就是run里面没有任何参数时使用的默认参数。如果我们想用默认参数，就直接run，否则想用其他参数，就run 里面加参数。