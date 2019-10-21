# **ClusterIP**

如何在已经运行的pod上创建service？

```shell
kubectl expose deployment service-test --port 8088 --target-port=8080
```

在名字为service-test的deployment所创建的pods上，创建cluster ip服务，服务的端口是8088。

ClusterIP 服务是 Kubernetes 的默认服务。它给你一个集群内的服务，集群内的其它应用都可以访问该服务。集群外部无法访问它。

ClusterIP 服务的 YAML 文件类似如下：

1. apiVersion: v1 
2. kind: Service 
3. metadata:   
4.   name: my-internal-service 
5. selector:     
6.   app: my-app 
7. spec: 
8.   type: ClusterIP 
9.   ports:   
10.   \- name: http 
11. ​    port: 80 
12. ​    targetPort: 80 
13. ​    protocol: TCP 

如果 从Internet 没法访问 ClusterIP 服务，那么我们为什么要讨论它呢？那是因为我们可以通过 Kubernetes 的 proxy 模式来访问该服务！

[![img](http://s5.51cto.com/oss/201804/12/87116f056e9071a1d76dc71a2b93c126.jpg)](http://s5.51cto.com/oss/201804/12/87116f056e9071a1d76dc71a2b93c126.jpg)

启动 Kubernetes proxy 模式：

1. $ kubectl proxy --port=8080 

这样你可以通过Kubernetes API，使用如下模式来访问这个服务：

1. http://localhost:8080/api/v1/proxy/namespaces/<NAMESPACE>/services/<SERVICE-NAME>:<PORT-NAME>/ 

要访问我们上面定义的服务，你可以使用如下地址：

1. http://localhost:8080/api/v1/proxy/namespaces/default/services/my-internal-service:http/ 

### **何时使用这种方式？**

有一些场景下，你得使用 Kubernetes 的 proxy 模式来访问你的服务：

- 由于某些原因，你需要调试你的服务，或者需要直接通过笔记本电脑去访问它们。
- 容许内部通信，展示内部仪表盘等。

这种方式要求我们运行 kubectl 作为一个未认证的用户，因此我们不能用这种方式把服务暴露到 internet 或者在生产环境使用。

# **NodePort**

NodePort 服务是引导外部流量到你的服务的最原始方式。NodePort，正如这个名字所示，在所有节点（虚拟机）上开放一个特定端口，任何发送到该端口的流量都被转发到对应服务。

[![img](http://s5.51cto.com/oss/201804/12/06794486d035874ef620b98b069f9d68.jpg)](http://s5.51cto.com/oss/201804/12/06794486d035874ef620b98b069f9d68.jpg)

NodePort 服务的 YAML 文件类似如下：

1. apiVersion: v1 
2. kind: Service 
3. metadata:   
4.   name: my-nodeport-service 
5. selector:     
6.   app: my-app 
7. spec: 
8.   type: NodePort 
9.   ports:   
10.   \- name: http 
11. ​    port: 80 
12. ​    targetPort: 80 
13. ​    nodePort: 30036 
14. ​    protocol: TCP 

NodePort 服务主要有两点区别于普通的“ClusterIP”服务。第一，它的类型是“NodePort”。有一个额外的端口，称为 nodePort，它指定节点上开放的端口值 。如果你不指定这个端口，系统将选择一个随机端口。大多数时候我们应该让 Kubernetes 来选择端口，因为如评论中 thockin 所说，用户自己来选择可用端口代价太大。

### **何时使用这种方式？**

1. 这种方法有许多缺点：
2. 每个端口只能是一种服务
3. 端口范围只能是 30000-32767

如果节点/VM 的 IP 地址发生变化，你需要能处理这种情况

基于以上原因，我不建议在生产环境上用这种方式暴露服务。如果你运行的服务不要求一直可用，或者对成本比较敏感，你可以使用这种方法。这样的应用的最佳例子是 demo 应用，或者某些临时应用。

# **LoadBalancer**

LoadBalancer 服务是暴露服务到 internet 的标准方式。在 GKE 上，这种方式会启动一个 Network Load Balancer[2]，它将给你一个单独的 IP 地址，转发所有流量到你的服务。

[![img](http://s3.51cto.com/oss/201804/12/ea0faab259a6d5a6ec1178e12e6cd295.jpg)](http://s3.51cto.com/oss/201804/12/ea0faab259a6d5a6ec1178e12e6cd295.jpg)

### **何时使用这种方式？**

如果你想要直接暴露服务，这就是默认方式。所有通往你指定的端口的流量都会被转发到对应的服务。它没有过滤条件，没有路由等。这意味着你几乎可以发送任何种类的流量到该服务，像 HTTP，TCP，UDP，Websocket，gRPC 或其它任意种类。

这个方式的最大缺点是每一个用 LoadBalancer 暴露的服务都会有它自己的 IP 地址，每个用到的 LoadBalancer 都需要付费，这将是非常昂贵的。

# **Ingress**

有别于以上所有例子，Ingress 事实上不是一种服务类型。相反，它处于多个服务的前端，扮演着“智能路由”或者集群入口的角色。

你可以用 Ingress 来做许多不同的事情，各种不同类型的 Ingress 控制器也有不同的能力。

GKE 上的默认 ingress 控制器是启动一个 HTTP(S) Load Balancer[3]。它允许你基于路径或者子域名来路由流量到后端服务。例如，你可以将任何发往域名 foo.yourdomain.com 的流量转到 foo 服务，将路径 yourdomain.com/bar/path 的流量转到 bar 服务。

[![img](http://s5.51cto.com/oss/201804/12/e3a3ac2d7720431dcffc16a4ae3ea497.jpg)](http://s5.51cto.com/oss/201804/12/e3a3ac2d7720431dcffc16a4ae3ea497.jpg)

GKE 上用 L7 HTTP Load Balancer[4]生成的 Ingress 对象的 YAML 文件类似如下：

1. apiVersion: extensions/v1beta1 
2. kind: Ingress 
3. metadata: 
4.   name: my-ingress 
5. spec: 
6.   backend: 
7. ​    serviceName: other 
8. ​    servicePort: 8080 
9.   rules: 
10.   \- host: foo.mydomain.com 
11. ​    http: 
12. ​      paths: 
13. ​      \- backend: 
14. ​          serviceName: foo 
15. ​          servicePort: 8080 
16.   \- host: mydomain.com 
17. ​    http: 
18. ​      paths: 
19. ​      \- path: /bar/* 
20. ​        backend: 
21. ​          serviceName: bar 
22. ​          servicePort: 8080 

### **何时使用这种方式？**

Ingress 可能是暴露服务的最强大方式，但同时也是最复杂的。Ingress 控制器有各种类型，包括 Google Cloud Load Balancer， Nginx，Contour，Istio，等等。它还有各种插件，比如 cert-manager[5]，它可以为你的服务自动提供 SSL 证书。

如果你想要使用同一个 IP 暴露多个服务，这些服务都是使用相同的七层协议（典型如 HTTP），那么Ingress 就是最有用的。如果你使用本地的 GCP 集成，你只需要为一个负载均衡器付费，且由于 Ingress是“智能”的，你还可以获取各种开箱即用的特性（比如 SSL、认证、路由等等）。

相关链接：

https://kubernetes.io/docs/concepts/services-networking/service/

https://cloud.google.com/compute/docs/load-balancing/network/

https://cloud.google.com/compute/docs/load-balancing/http/

https://cloud.google.com/compute/docs/load-balancing/http/

https://github.com/jetstack/cert-manager