# rpc

rpc采用go自带的net/rpc包提供rpc服务

### 基本思路

#### server

调用rpc.register函数把满足规则的rpc方法注册进来，然后listen/accpet建立tcp连接conn，最后调用rpc.serveconn方法在conn上提供注册过的rpc服务。

#### client

直接调用rpc.dial方法连通rpc服务，该方法返回的是rpc client，通过此rpc client可以直接调用rpc服务方法。

以上都是采用默认的go语言中gob编码的情况。

### jsonrpc

如果采用json编码的话，是有变化的。

对于server端，区别是不再调用rpc.serverconn方法了，调用rpc.serverconn时默认采用了gob编码，那肯定是不行的。采用的方法是

```go
rpc.ServeCodec(jsonrpc.NewServerCodec(conn))
```

其中conn和上面的conn是同一个，但是在conn上进行了json的编解码。

对于client端，区别是不再采用rpc.dial了，原因和上面的server一样，rpc.dial默认是按照gob编码方式来连接rpc服务的，对于json编码的数据肯定不行。代替方法是

```go
client := rpc.NewClientWithCodec(jsonrpc.NewClientCodec(conn))
```

很明显，基于conn链接建立了json的编解码器。

### rpc via http

上面的conn是tcp链接，如果采用http链接呢？

也很简单，思路是只把http看作提供服务的一种形式，也就是说把http看作收集数据的一种方法，http只负责把数据收集进来。对于收集进来的数据，如果是按照json编码，则可以参考上面jsonrpc实现策略，对数据进行jsoncodec封装即可。

举例

```go
func main() {
    rpc.RegisterName("HelloService", new(HelloService))

    http.HandleFunc("/jsonrpc", func(w http.ResponseWriter, r *http.Request) {
        var conn io.ReadWriteCloser = struct {
            io.Writer
            io.ReadCloser
        }{
            ReadCloser: r.Body,
            Writer:     w,
        }

        rpc.ServeRequest(jsonrpc.NewServerCodec(conn)) // conn既是链接，也是数据，因为数据绑定在了链接上
    })

    http.ListenAndServe(":1234", nil)
}
```

# grpc

grpc默认采用http2/2协议

#### server

和标准库的net/rpc相比，首先是通过`grpc.NewServer()`构造一个gRPC服务对象，然后通过gRPC插件生成的RegisterHelloServiceServer函数注册我们实现的HelloServiceImpl服务。然后通过`grpcServer.Serve(lis)`在一个监听端口上提供gRPC服务。

#### client

采用grpc.Dial和gRPC服务建立链接，然后NewHelloServiceClient函数基于已经建立的链接构造HelloServiceClient对象。返回的client其实是一个HelloServiceClient接口对象，通过接口定义的方法就可以调用服务端对应的gRPC服务提供的方法。

# 备注

1，rpc方法需要满足的规则

方法只能有两个可序列化的参数，其中第二个参数是指针类型，并且返回一个error类型，同时必须是公开的方法。

