# 第6次实验课实验报告要求-VPN实验课上





## 一、实验内容-课上



 1.Windows IPSec配置 

2.抓包分析 IKE SA和IPSec SA建立过程



## 二、实验准备工作

 课上： 两台Windows虚拟机 或一台Windows虚拟机+Windows物理主机 （指导书操作以虚拟机+物理主机为例）



##  三、课上实验步骤 



### 1.Windows配置IPSec 

（1）使用Win+R打开本地安全设置模块，在IP安全策略创建新IP安全策略 

![image-20231204154809485](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204154809485.png)

（2）新建的IP安全策略名称自拟，勾选“激活默认响应规则”，使用“123456”（或其他）作为预共享密钥保护密钥交换，指派该策略。 

![image-20231204154903186](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204154903186.png)

![image-20231204154918141](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204154918141.png)

![image-20231204155035002](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204155035002.png)

![image-20231204163159955](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204163159955.png)

还是用123456吧

（3）修改新建策略的属性，添加安全规则。不指定隧道终结点，适用于所有网络连接，使用“abcdef”作为预共享密钥保护密钥交换。 

![image-20231204155206527](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204155206527.png)

![image-20231204155220796](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204155220796.png)

![image-20231204155431006](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204155431006.png)

（4）添加IP筛选器列表，在新的筛选器列表添加新的筛选器，然后添加筛选器操作。在筛选器列表选择“所有IP通讯量”。 

![image-20231204155521913](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204155521913.png)

（5）在筛选器操作界面选择“需要安全”，然后完成修改属性。

![image-20231204155543813](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204155543813.png)

 （6）在另一台Windows主机上执行相似的操作。至此，IPSec已配置完成。 

### 2.抓包分析 IKE SA和IPSec SA建立过程 

![图片1](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/图片1.png)

（1）ipconfig查看主机IP，使用一台主机ping另一台 

![image-20231204163333628](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204163333628.png)

![image-20231204163358423](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204163358423.png)

（2）在任意一台主机上使用WireShark抓包 

![image-20231204163513875](C:/Users/24328/AppData/Roaming/Typora/typora-user-images/image-20231204163513875.png)

![image-20231204164444373](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204164444373.png)

（3）查找资料，结合抓到的数据包分析IKE Sec和IPSec SA建立过程



### 3.分析 IKE SA 和 IPSEC SA 建立过程

要建立 `IPSec连接`，首先要协商一个 `IKE SA`，然后在 `IKE SA` 的基础上协商 `IPSec SA`

#### IKE SA过程



IKE SA 建立分为三个阶段

![image-20231204164618119](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204164618119.png)

1.`SA交换`，协商确认有关安全策略。该过程进行安全协商

![image-20231204164747674](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204164747674.png)

![image-20231204164824886](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204164824886.png)

2.密钥交换阶段，主要交换密钥 `Diffie-Hellman公共值`。数据包中的 `Key Exchange` 用于交换各自加密生成的`主密钥`；`Nonce` 使用了随机数，防止重放攻击；加密所用的密钥为 `ipsec` 中设定的`预共享密钥`； `NAT-D` 为双方的 ip + 端口的 Hash 值。

![image-20231204164854792](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204164854792.png)

![image-20231204164924049](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204164924049.png)

3.ID 信息和认证数据交换，进行身份认证，对第一阶段交换内容的认证。

![image-20231204165008818](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204165008818.png)

![image-20231204165022551](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204165022551.png)

#### IPSec SA

`IPSec SA` 建立分为两个阶段，都是加密数据，无法查看。用到了 `Quick-Mode`，目的是在两个对等体间协商一组一致的参数来创建 `IPSec SA`，用于真实数据的加解密，并且在此进行 `PFS`，`PFS` 及在 `Quick-Mode` 重新做 `DH` 的交换，产生新的密钥用于 `IPSec` 数据的加密。

![image-20231204165102784](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204165102784.png)

![image-20231204165118138](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204165118138.png)

如下所示的ESP部分的数据包便是建立IPSec之后的加密通信数据包：

![image-20231204165320714](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231204165320714.png)

至此实验分析完成。

### 总结一下IKE Sec和IPSec SA

IKE（Internet Key Exchange）和IPSec（Internet Protocol Security）是用于在网络通信中实现安全性的协议套件。它们通常一起使用，以确保数据的保密性、完整性和身份验证。在了解IKE和IPSec之前，让我们先了解一下两个重要的概念：SA（Security Association）和ISAKMP（Internet Security Association and Key Management Protocol）。

1. **安全关联（Security Association - SA）**：
   - SA 是在通信双方之间建立的安全参数集合，用于保护通信。
   - 它包括了安全性协议的参数，如加密算法、认证算法、密钥生命周期等。
   - 对于每个通信方向，都存在一个SA。
2. **ISAKMP（Internet Security Association and Key Management Protocol）**：
   - ISAKMP 是一种协议，用于在通信双方之间建立和管理安全关联。
   - 它定义了在两个对等体之间协商安全性参数的步骤。

接下来，我们讨论IKE和IPSec SA的建立过程：

**IKE（Internet Key Exchange）**：

1. **阶段一（Phase 1）**：
   - 在这个阶段，IKE 首先确立一个安全的通信通道，这个通道称为ISAKMP SA。
   - 通信双方协商使用的加密和认证算法，并交换密钥。
   - 主要目标是建立一个安全的通信通道，以便在接下来的阶段中交换IPSec SA 的相关信息。
2. **阶段二（Phase 2）**：
   - 在这个阶段，使用 ISAKMP SA 建立的安全通道，通信双方协商 IPsec SA 的参数。
   - 这包括通信所使用的加密算法、认证算法、密钥生命周期等。
   - 当 IPsec SA 被建立后，通信双方就可以在安全通道上传输加密的数据。

**IPSec SA（Security Association）**：

1. **初始化**：
   - 通信双方使用已经建立的 ISAKMP SA 来协商 IPsec SA 的参数。
   - 这些参数包括加密算法、认证算法、散列算法等。
2. **协商**：
   - 通信双方协商各自的安全关联，包括源和目标IP地址，以及使用的加密和认证算法。
3. **建立**：
   - 通过 ISAKMP SA 建立的安全通道，通信双方交换 IPsec SA 的相关信息，从而建立安全通信通道。
4. **维护**：
   - 定期更新密钥和安全关联参数，以确保通信的安全性和完整性。

总体而言，IKE 和IPSec 通过阶段一和阶段二的协商过程，以及使用 ISAKMP SA 和 IPsec SA，确保了网络通信的安全性。这包括了密钥的安全协商、数据的加密和认证。