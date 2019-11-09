python3安装前，最好先安装下依赖包：

yum install -y openssl-devel

yum install -y openssl

yum install -y zlib-devel bzip2-devel openssl-devel ncurses-devel sqlite-devel readline-devel tk-devel gdbm-devel db4-devel libpcap-devel xz-devel

$ yum install gcc -y

$ yum install zlib zlib-devel -y

 

$ wget https://www.python.org/ftp/python/3.6.2/Python-3.6.2rc1.tgz



$ tar zxvf Python-3.6.2rc1.tgz

$ cd Python-3.6.2rc1/

把Python3.6安装到 /usr/local 目录

$ ./configure --prefix=/usr/local

$ make

$ make install

最后提示：Successfully installed pip-9.0.1 setuptools-28.8.0



$ ln -s /usr/local/bin/python3.6 /usr/bin/python3

$ ln -s /usr/local/bin/python3.6 /usr/bin/python #最好不要把原本的旧版python覆盖掉。

 

在使用pip3.6安装模块的时候出现如下错误。

```shell
`[root@zabbix Python-3.6.2rc1]# pip install ansible``pip ``is` `configured ``with` `locations that require TLS/SSL, however the ssl module ``in` `Python ``is` `not` `available.``Collecting ansible``  ``Retrying (Retry(total=4, ``connect``=None, ``read``=None, redirect=None, status=None)) ``after` `connection` `broken ``by` `'SSLError("Can'``t ``connect` `to` `HTTPS URL because the SSL module ``is` `not` `available.",)``': /simple/ansible/``  ``Retrying (Retry(total=3, connect=None, read=None, redirect=None, status=None)) after connection broken by '``SSLError(``"Can't connect to HTTPS URL because the SSL module is not available."``,)': /simple/ansible/``  ``Retrying (Retry(total=2, ``connect``=None, ``read``=None, redirect=None, status=None)) ``after` `connection` `broken ``by` `'SSLError("Can'``t ``connect` `to` `HTTPS URL because the SSL module ``is` `not` `available.",)``': /simple/ansible/``  ``Retrying (Retry(total=1, connect=None, read=None, redirect=None, status=None)) after connection broken by '``SSLError(``"Can't connect to HTTPS URL because the SSL module is not available."``,)': /simple/ansible/``  ``Retrying (Retry(total=0, ``connect``=None, ``read``=None, redirect=None, status=None)) ``after` `connection` `broken ``by` `'SSLError("Can'``t ``connect` `to` `HTTPS URL because the SSL module ``is` `not` `available.",)``': /simple/ansible/``  ``Could not fetch URL https://pypi.org/simple/ansible/: There was a problem confirming the ssl certificate: HTTPSConnectionPool(host='``pypi.org``', port=443): Max retries exceeded with url: /simple/ansible/ (Caused by SSLError("Can'``t ``connect` `to` `HTTPS URL because the SSL module ``is` `not` `available.",)) - skipping``  ``Could ``not` `find a version that satisfies the requirement ansible (``from` `versions: )``No` `matching distribution found ``for` `ansible``pip ``is` `configured ``with` `locations that require TLS/SSL, however the ssl module ``in` `Python ``is` `not` `available.``Could ``not` `fetch` `URL https://pypi.org/simple/pip/: There was a problem confirming the ssl certificate: HTTPSConnectionPool(host=``'pypi.org'``, port=443): ``Max` `retries exceeded ``with` `url: /simple/pip/ (Caused ``by` `SSLError(``"Can't connect to HTTPS URL because the SSL module is not available."``,)) - skipping``[root@zabbix Python-3.6.2rc1]#`
```

　　

　　从错误提示来看是缺少了ssl模块，centos下的解决方案...

```shell
`yum install openssl``yum install openssl-devel -y`
```



