为什么会出现websocket？websocket解决了http存在的什么问题？

http有一些问题，为了解决这些问题，websocket出现了：

1，请求/响应是单向的，只能客户端主动发送请求，服务端被动的通过响应将消息回复给客户端；

2，为了实现即时通信，采用的技术都是“轮询”，即在特定的时间间隔内，由浏览器对服务器发出HTTP Request，服务器在收到请求后，返回最新的数据给浏览器刷新。当服务端长时间不需要向浏览器发送消息，但此时“轮询”机制依然存在，浏览器还是会间隔一定时间向服务端发送请求，此时浪费带宽，而且会造成服务端不得不回复无用消息，浪费服务器资源；

3，由于http本身无状态，若要保持状态，则需要通过cache session进行，造成每次数据请求时会携带大量信息；



websocket好处：

- 一个Web客户端只建立一个TCP连接；
- Websocket服务端可以推送(push)数据到web客户端；
- 有更加轻量级的头，减少数据传送量。通过协议promote的方式将http升级成websocket。浏览器的请求头中有upgrade: websocket; connection: upgrade; sec-websocket-key: id; sec-websocket-protocol; sec-websocket-version，服务器回复消息中包含upgrade: websocket; connection: upgrade; sec-websocket-accept: id。其中，回复中的sec-websocket-accept: id，其中的id是根据请求中的sec-websocket-key: id 中的id，再加上一个字符串258EAFA5-E914-47DA-95CA-C5AB0DC85B11拼接完毕，并按照base64编码构造的。