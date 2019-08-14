简介
        最近从eclipse转到IDEA，一开始就遇到问题，以前eclipse运行的MAVEN管理的程序，在IDEA中，总要报错NoClassDefFoundError比如:
Exception in thread "main" java.lang.NoClassDefFoundError: org/apache/spark/api/java/function/Function2
    at java.lang.Class.forName0(Native Method)
    at java.lang.Class.forName(Class.java:264)
    at com.intellij.rt.execution.application.AppMain.main(AppMain.java:122)
Caused by: java.lang.ClassNotFoundException: org.apache.spark.api.java.function.Function2
    at java.net.URLClassLoader.findClass(URLClassLoader.java:381)
    at java.lang.ClassLoader.loadClass(ClassLoader.java:424)
    at sun.misc.Launcher$AppClassLoader.loadClass(Launcher.java:331)
    at java.lang.ClassLoader.loadClass(ClassLoader.java:357)
    ... 3 more
       
问题解析
         一时间相当郁闷，明明都引入了包了，在eclipse中都能运行，在IDEA中为何不行。一番研究后终于发现问题所在出现于此：
        在我的pom文件中是这样的：

<dependency>
   <groupId>org.apache.spark</groupId>
   <artifactId>spark-core_2.10</artifactId>
   <version>2.1.0</version>
   <scope>provided</scope>
</dependency>
        这在eclipse中完全没问题，但是在IDEA中，maven配置<scope>provided</scope>，依赖只作用于编译和打包等，但不能用于运行类。通俗的说，配置了<scope>provided</scope>，就是在告诉IDEA，你不要担心，我会在运行时提供这个依赖，但是实际上却并没有提供依赖。除非将这个依赖加到类路径下，那么就OK了。所以解决办法也就出来了.

解决办法
        1.保留<scope>provided</scope>配置项，然后手动将依赖加到类路径。
                       如何添加依赖参看http://blog.csdn.net/a153375250/article/details/50851049
                       或者这样也行：File–>Project Structure–>左侧 Libraries，中间新建一个 lib 的project Library，选择 Java，然后在选择项目中的 jar 所在的文件夹

            2.去掉<scope>provided</scope>，改为<scope>compile</scope>，当然只去掉也可，默认就是compile。注：改完后，在maven上运行一下reimport，重新导入一下。打完收工。
---------------------
版权声明：本文为CSDN博主「疯狂的暴走蜗牛」的原创文章，遵循CC 4.0 by-sa版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/u010936936/article/details/78499667