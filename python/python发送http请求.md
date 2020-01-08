**前言**

在运维过程中，我们可能需要通过某些http接口提交一些数据到我们的资产管理系统、监控系统等，我们可以使用python的第三方库requests来进行操作，优雅而简单。

![img](https://ss2.baidu.com/6ONYsjip0QIZ8tyhnq/it/u=4188117302,799359565&fm=173&app=25&f=JPEG?w=640&h=821&s=0AAA7A22BB7DD3EF1F5DB4CF0100E0A0)



**安装**

直接使用pip就可以安装：

pip install requests



**使用**

1、发送请求

import requests #导入requests，然后就可以为所欲为了

\#发送get请求

r0 = requests.get("http://yunweicai.com")

\#发送post请求

r1 = requests.post("http://yunweicai.com",data={key:value})

\#发送post请求，带json串

json_data = {"user":"yunweicai","op":"post"}

r11 = requesets.post("http://yunweicai.com",json=json_data)

\#put、delete、head、optiions请求也很简单

r = requests.put('http://yunweicai.com/put', data = {'key':'value'})

r = requests.delete('http://yunweicai.com/delete')

r = requests.head('http://yunweicai.com/get')

r = requests.options('http://yunweicai.com/get')

2、URL参数

URL 的查询字符串(query string)传递某种数据。如果你是手工构建 URL，那么数据会以键/值对的形式置于 URL 中，跟在一个问号的后面。例如， yunweicai.com/get?key=val。

requests库操作就比较优雅了，requests 允许你使用 params 关键字参数，以一个字符串字典来提供这些参数。

payload = {'key1': 'value1', 'key2': 'value2'}

r = requests.get("http://yunweicai.com/get", params=payload)

通过打印输出该 URL，你能看到 URL 已被正确编码：

print(r.url)

3、相应内容

通过发送请求返回的对象，我们就可以获取到服务器对我们的相应内容了。Requests 会自动解码来自服务器的内容。请求发出后，Requests 会基于 HTTP 头部对响应的编码作出有根据的推测。当你访问 r.text 之时，Requests 会使用其推测的文本编码。

你可以找出 Requests 使用了什么编码，并且能够使用 r.encoding 属性来改变它:

\>>> r.encoding'utf-8'>>> r.encoding = 'ISO-8859-1'

如果返回的json串，可以直接使用r.json()获取到字典对象进行操作

如果响应内容是二进制呢？

![img](https://ss0.baidu.com/6ONWsjip0QIZ8tyhnq/it/u=2527520025,1168290015&fm=173&app=25&f=JPEG?w=640&h=404&s=FD90C910BB7AE1CE1A4151C20100E0B0)

对于响应内容是非文本请求，可以使用r.content访问到内容。

例如，以请求返回的二进制数据创建一张图片，你可以使用如下代码：

\>>> from PIL import Image>>> from io import BytesIO>>> i = Image.open(BytesIO(r.content))

4、定制请求头

有些请求需要有指定的请求头才能正确获取到内容。

headers = {'user-agent': 'my-app/0.0.1'}

r= requests.get("http://yunweicai.com",headers=headers)

5、cookies

有些请求需要依赖于cookies来完成操作，就需要从上个请求中获取到cookies然后传入下一个请求：

r0 = requests.get("http://yunweicai.com/login")

r1 = requests.post("http://yunweicai.com/asset",data={"hostname":"yunweicai",cookies= r0.cookies}



**总结**

使用requests是不是觉得很简单而优雅呢？

人生苦短，我用Python！

![img](https://ss0.baidu.com/6ONWsjip0QIZ8tyhnq/it/u=3189252984,21945061&fm=173&app=25&f=JPEG?w=582&h=271&s=6B94EC0A96A078BAC2CD4DDB010010A2)