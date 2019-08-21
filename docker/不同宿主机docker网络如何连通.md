不同宿主机docker如何进行网络连通

docker启动时，会在宿主主机上创建一个名为docker0的虚拟网络接口，默认选择172.17.42.1/16，一个16位的子网掩码给容器提供了65534个IP地址。docker0只是一个在绑定到这上面的其他网卡间自动转发数据包的虚拟以太网桥，它可以使容器和主机相互通信，容器与容器间通信。

问题是，如何让位于不同主机上的docker容器可以通信？

最简单的思路，修改一台主机docker默认的虚拟网段，然后在各自主机上分别把对方的docker网段加入到路由表中，即可实现docker容器夸主机通信。

现有两台虚拟机

- v1：192.168.124.51
- v2：192.168.124.52

更改虚拟机docker0网段，修改为

- v1：172.17.1.1/24
- v2：172.17.2.1/24



`

#v1
sudo ifconfig docker0 172.17.1.1 netmask 255.255.255.0
sudo service docker restart

#v2
sudo ifconfig docker0 172.17.2.1 netmask 255.255.255.0
sudo service docker restart

`



然后在v1，v2上把对方的docker0网段加入到自己的路由表中



`

#v1
sudo route add -net 172.17.2.0 netmask 255.255.255.0 gw 192.168.124.52
sudo iptables -t nat -F POSTROUTING
sudo iptables -t nat -A POSTROUTING -s 172.17.1.0/24 ! -d 172.17.0.0/16 -j MASQUERADE

#v2
sudo route add -net 172.17.1.0  netmask 255.255.255.0  gw 192.168.124.51
sudo iptables -t nat -F POSTROUTING
sudo iptables -t nat -A POSTROUTING -s 172.17.2.0/24 ! -d 172.17.0.0/16 -j MASQUERADE

`

测试，v1，v2创建容器test1，test2

`

#v1
docker run --rm --name test1 -i -t base:latest bin/bash
docker inspect --format '{{.NetworkSettings.IPAddress}}' test1

#172.17.1.1

#v2
docker run --rm --name test2 -i -t base:latest bin/bash
docker inspect --format '{{.NetworkSettings.IPAddress}}' test2

#172.17.2.1

`

主机上可以ping通对方容器ip，至此也就ok了