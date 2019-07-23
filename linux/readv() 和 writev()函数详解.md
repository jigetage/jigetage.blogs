直接上结论：

readv()代表分散读， 即将数据从文件描述符读到分散的内存块中；

writev()代表集中写，即将多块分散的内存一并写入文件描述符中。



依赖项：

头文件：include <sys/uio.h>

api接口：

ssize_t readv(int fd, const struct iovec *vector, int count);
ssize_t writev(int fd, const struct iovec *vector, int count);

fd参数是被操作的文件描述符。
vector参数是iovec结构体：

struct iovec
{
	void *iov_base;  //指向一个缓冲区，这个缓冲区是存放readv()所接收的数据或	                   								  	        //writev()将要发送的数据
	size_t iov_len;  //接收的最大长度以及实际写入的长度
};

count参数是vector数组的长度，即有多少块内存数据需要从fd读出或写到fd。

两者调用成功是返回读出/写入fd的字节数，失败返回-1，并设置errno，此时需要引入error.h头文件。