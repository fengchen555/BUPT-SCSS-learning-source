# 第6次实验课实验报告要求-VPN实验课下





## 一、实验内容-课下



课下

CiscoPacketTracer中VPN配置实验



## 二、实验准备工作

课下：

安装CiscoPacketTracer



##  三、课上实验步骤 



### 1.使用CiscoPacketTracer

![image-20231204171850151](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204171850151.png)

###  2.搭建网络环境 



#### （1）初始化配置路由器。



 打开cisco模拟器，在模拟器窗口工具栏下选择file-new。在左下角设备栏选取路由器图标，将cisco2811路由器拖到工作区域。单击工作区域的路由器图标，选择CLI项，弹出界面，等待路由器启动后，在弹出的交互对话中，输入“n”回车，然后再按回车。

![image-20231204172134394](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204172134394.png)



 分别进入路由器router0和router1的特权模式配置路由器，输入命令。

Router>enable     #进入特权模式，只有在特权模式下才可以对路由器进行配置



/*********配置网卡f0/0端口的IP地址和子网掩码,该网卡连接公网*********/

Router#configure terminal          #进入配置状态，通过端口进行配置

Enter configuration commands, one per line.  End with CNTL/Z.

Router(config)# interface fastEthernet 0/0  #进入端口f0/0,配置网卡f0/0的ip地址和子网掩码

Router(config-if)#ip address 10.0.0.1 255.255.255.0 #配置网卡f0/0的ip地址和子网掩码

Router(config-if)#no shutdown            #开启端口f0/0

Router(config-if)#end                #返回特权模式

![image-20231204173049024](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204173049024.png)



/***********配置网卡f0/1的IP地址和子网掩码,该网卡连接内网************/

Router# configure terminal         #进入配置状态，通过端口进行配置

Enter configuration commands, one per line.  End with CNTL/Z.

Router(config)# interface fastEthernet 0/1  #配置网卡f0/0的ip地址和子网掩码

Router(config-if)#ip address 192.168.1.1 255.255.255.0 #配置网卡f0/0的ip地址和子网掩码

Router(config-if)#no shutdown              #开启端口f0/0

Router(config-if)# end                  #返回特权模式

初始配置router 0完成，根据router 0的配置过程完成router 1的配置，其中router 1的f0/0端口IP为10.0.0.2/24, router 1 f0/1端口的IP地址为192.168.2.1/24。

![image-20231204191740681](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204191740681.png)



 

配置完成后，点击![img](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/wps1.jpg)，选择![img](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/wps2.jpg)，将router 0和router 1的f0/0端口连接

![image-20231204173651536](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204173651536.png)

#### （2）在模拟器左下角选择 ![img](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/wps3.jpg)，选取![img](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/wps4.jpg)拖到绘图工作区，双击PC图标，选择Desktop，选择IP Configuration,配置PC的IP地址和子网掩码。选取交换机![img](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/wps5.jpg)拖至工作区，在![img](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/wps6.jpg)选取![img](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/wps7.jpg)将路由器于交换机相连，将交换机于PC相连，最终完成网络。





#### （3）配置路由器，使路由器两端网络互通。

配置router 0：

双击router 0图标，选择CLI项，进入路由器配置窗口:

Router>en

Router#configure terminal 

Enter configuration commands, one per line.  End with CNTL/Z.

Router(config)# ip route  0.0.0.0  0.0.0.0  fastEthernet  0/0  #配置内网访问外部网络的出口路由

Router(config)#ip route  192.168.1.0  255.255.255.0  fastEthernet 0/1 #配置外部访问内部网络入口路由

Router(config)#end

![image-20231204200448102](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204200448102.png)

配置router 1：

Router>en

Router#conf t

Enter configuration commands, one per line.  End with CNTL/Z.

Router(config)#ip route 192.168.1.0 255.255.255.0 f

Router(config)# ip route  0.0.0.0 0.0.0.0  fastEthernet 0/0  #配置内网访问外部网络出口路由

Router(config)# ip route 192.168.2.0 255.255.255.0 fastEthernet 0/1 #配置外部访问内部网络入口路由

Router(config)#end

 

![image-20231204200401186](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204200401186.png)

#### （4）测试网络互通性 双击PC 0图标，在弹出的对话框中，选择Desktop，选择Command Prompt ，通过ping测试网络连通性，结果表明搭建的网络满足实验环境。

 -PC0能ping通router0的内外网地址 

![image-20231204200530687](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204200530687.png)

-PC0能ping通router1的内外网地址 

![image-20231204200559306](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204200559306.png)

-PC0能ping通router1连接的主机地址。

![image-20231204200631499](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204200631499.png)





###  3.配置IPSec VPN 

#### （1）配置router0 定义IKE；定义数据加密方式和认证方式，配置IPSec；将map映射到公网端口 



a）定义的IKE的策略,IKE就是router 0和router1之间的密钥交换策略：

 Router (config)#crypto isakmp policy 10            #一个IKE的策略，号码是10，数字越低，策略优先级越高 Router (config-isakmp)# authentication pre-share   #使用预定义共享密钥进行设备认证 Router (config-isakmp)#hash md5                   #认证方式使用MD5进行认证 Router (config-isakmp)#encryption des              #加密方式使用DES，可选AES/DES Router (config-isakmp)#group 2                    #指定DH组 Router (config-isakmp)# lifetime 86400             #注释：对生成新SA的周期进行调   整，两端的路由器都要设置相同的SA周期。 Router (config-isakmp)# exit Router (config)#crypto isakmp key bupt address 10.0.0.2   #定义一个密码,密码是bupt，和地址为10.0.0.2的设备去交换密钥。 IKE只是密钥的交换策略,我们在使用加密对称和非对称加密算法的时候,需要密钥来对数据加密,上面的IKE策略只是建立一条管理连接，负责加密生成的各种密钥。

![image-20231204201002009](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204201002009.png)

 b）定义数据的加密方式和认证方式，配置IPSec Router (config)

#access-list 110 permit ip 192.168.1.0 0.0.0.255 192.168.2.0 0.0.0.255 #该条配置定义出访问控制列表,这里的访问控制列表不是对数据进行过滤，是定义那些数据应该被加密，也可以理解哪些数据触发IPSec 流。 Router (config)#crypto ipsec transform-set mine esp-des esp-md5-hmac #该配置设置数据的加密方式，策略名字为mine，使用ESP-DES对数据加密，ESP-MD5-HMAC对数据认证。 Router(config)# crypto map mymap 101 ipsec-isakmp  #定义一个map,来调用我们刚才做的策略。 Router(config-crypto-map)# match address 110       #匹配出访问控制列表110的数据 Router(config-crypto-map)# set peer 10.0.0.2      #标识对端路由器的合法IP地址 Router(config-crypto-map)# set pfs group2 Router(config-crypto-map)# set transform-set mine  #使用什么方式对匹配的数据加密，使用刚才定义好的策略对数据加密。 Router(config-crypto-map)# set security-association lifetime seconds 86400 #指定IPSec SA的存活期

![image-20231204201134351](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204201134351.png)

 c）将map映射到公网端口，一个端口只能映射一个map

 Router（config）interface fastEthernet 0/0

 Router(config-if)#crypto map mymap 

Jan  3 07:16:26.785: %CRYPTO-6-ISAKMP_ON_OFF: ISAKMP is ON 

Router（config-if）end 

![image-20231204201509661](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204201509661.png)

通过show命令，查看配置好的策略。 

![image-20231204201440845](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204201440845.png)

#### （2）用相似的方式配置router1



![image-20231204201910130](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204201910130.png)



###  4.验证配置结果 

#### （1）测试VPN连通性

 双击PC0图标，在弹出的对话框中，选择Desktop，选择Command Prompt  ，ping 192.168.2.10 

![image-20231204202136004](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204202136004.png)

#### （2）验证数据经过IPSec VPN加密传输

 进入simulation mode，重复ping操作，在simulation Panel中选取Auto Capture，观察工作区动画，双击在路由器router 0处数据包，分析数据包的信息。 

![image-20231204205709115](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204205709115.png)

![image-20231204213748159](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204213748159.png)

进入路由器的数据包（左侧）的信息源IP是192.168.1.10，目的IP是192.168.2.10，但从路由器出去的数据包的源IP改变为10.0.0.1，目的IP变为10.0.0.2，同时在第6条信息中可以看到ESP encrypts the received packet的信息。由此看出从PC0（192.168.1.10）发往对端PC3（192.168.2.10）的数据经过了路由器的IPSec VPN模块加密处理，隐藏了内网的IP地址信息，从而保护了内网的数据

![image-20231204213849703](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204213849703.png)

![image-20231204213946102](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204213946102.png)

假如有一方不用加密：

![image-20231204214745506](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204214745506.png)

![image-20231204214754718](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204214754718.png)

发现此时无法ping通

假如两个都不进行加密，又可以ping通
![image-20231204214921358](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204214921358.png)

重新加密；
![image-20231204214956780](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204214956780.png)

可以看到重新ping通并看到了**IPSec**

![image-20231204215126410](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204215126410.png)

在一次成功加密通信后，就一直使用这个进行通信。

## 四、课下实验总结

通过本次课下的VPN配置实验，我深入了解了CiscoPacketTracer中VPN的配置过程。以下是我对实验的总结和体会：

1. **实验目标达成：** 实验的主要目标是在CiscoPacketTracer中配置VPN，包括初始化路由器、搭建网络环境、配置IPSec VPN等步骤。通过实验，我成功完成了这些配置步骤，并验证了VPN的连通性。
2. **网络拓扑搭建：** 在实验中，我学会了使用CiscoPacketTracer搭建网络拓扑，包括选择路由器、PC、交换机等设备，并通过连接它们来构建一个简单的网络环境。
3. **路由器配置：** 实验中涉及了路由器的初始化配置，包括配置路由器接口的IP地址和子网掩码，设置路由表以实现网络互通。这为后续的VPN配置提供了基础。
4. **IPSec VPN配置：** 配置IPSec VPN是实验的重点，包括定义IKE策略、配置数据加密方式和认证方式、将map映射到公网端口等步骤。通过这些配置，成功建立了安全的VPN连接。
5. **验证与分析：** 在实验中，我学会了使用命令行和模拟模式验证配置的正确性。通过Ping测试和分析数据包，我确认了VPN的连通性以及数据经过IPSec VPN加密传输的情况。
6. **问题解决能力：** 在实验过程中，遇到了一些问题，例如配置中的错误和连通性测试失败。通过仔细排查和调试，我解决了这些问题，提高了自己的问题解决能力。

总的来说，本次实验加深了我对VPN配置的理解，提升了我在网络配置方面的实际操作能力。通过手动配置VPN，我更好地理解了网络安全的重要性以及如何保障数据传输的安全性。这对于今后从事网络工程和安全相关工作都具有实际意义。