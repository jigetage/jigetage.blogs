在配置文件docker-compose.yml的services字段中，添加command字段，然后在command的命令后面按照格式输入启动参数即可。

例如，设置prometheus热加载配置文件可以通过如下方式：

 13   prom:
 14     build:
 15       context: prom/
 16     ports:
 17       - 9090:9090
 18     volumes:
 19       - ./prom/config/prometheus.yml:/etc/prometheus/prometheus.yml
 20       - ./prom/:/prometheus
 21     restart: on-failure
 22     command: --web.enable-lifecycle --config.file=/etc/prometheus/prometheus.yml

