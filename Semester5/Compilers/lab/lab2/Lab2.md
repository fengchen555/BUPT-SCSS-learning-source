



打开文件目录，并输入代码

```c
所给程序：
make wc
./wc.out inferno3.txt 
linux统计：
wc inferno3.txt 
```

![image-20230930162109961](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930162109961.png)

发现所给程序的的wc命令与linux的wc命令区别在于**输出统计单词数上有区别**(输出的间隔就不说了)。

linux的wc原理：

`wc` 命令的工作原理如下：

1. 打开指定的文本文件。
2. 逐行读取文件的内容，并统计行数。
3. **对每一行的内容进行分割，使用空格、制表符等空白字符作为分隔符，然后统计分割后的单词数**。
4. 统计文件中的字符数。
5. 输出统计结果到标准输出

所给程序统计单词数：

统计字数的方式是通过正则表达式`{letter}+`匹配一个或多个字母，每次匹配到时就将`words`变量加1，同时将当前匹配到的文本长度加到`chars`变量上。

例如这样的数据："hello,world"

![image-20230930165945257](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930165945257.png)

结果：

![image-20230930170032099](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930170032099.png)

其实就是wc命令只是根据空格换行来判断单词，而我们的程序时根据对字母表中的匹配实现的，要更加精准。

## Flex Exercises - identifiers

运行代码：

```
make idcount 
./idcount.out ./test.c
```

未修改前：

![image-20230930170618560](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930170618560.png)

我们发现可以识别输入文本中的有效标识符，但是打印出来行数是却是错误的。

![image-20230930171638350](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930171638350.png)

注意看这里的代码，当识别到\n也就是换行时发现竟然没有任何其他的操作，理论上应该lines++，所以错误应该发生在这，我们修改为：
![image-20230930171747509](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930171747509.png)

加上lines++；对识别到换行是行数+1。然后make idcount ，./idcount.out ./test.c。

![image-20230930171916315](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930171916315.png)

发现就可以成功识别行数，输出中的标记和行号成功匹配。至此这部分实验结束。

## Flex Exercise - ipaddr

运行代码：
![image-20230930172749509](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930172749509.png)

发现没能通过。我们先看看测试集（在python代码中输出test_cases）：

![image-20230930173517180](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930173517180.png)

###  ipv4



我们查看这些数据，并且我们可以知道，ipv4是0-255.0-255.0-255，然后我们可以用最简单的方法，将0-255分为5部分，200-255、0-9、10-99、100-199。即为**(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])**就是匹配对应的数字，然后匹配点号（`.`）即:**(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9]).**重复三次，最后末尾也为**(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])**。所以最后的正则即为：

***^((25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[0-9])\\.){3}(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9][0-9]|[1-9])$***

### ipv6

判断一个地址是否为IPv6地址的条件是：

- 地址长度为 8 个 16进制数，每个数之间用冒号分隔。
- 每个 16 进制数由四个十六进制位（4 bit）组成，范围为 0000 ~ FFFF。
- 每个 16 进制数的前导零可以省略，但至少有一个数不能省略前导零。
- 地址中允许使用 "::" 来表示多个连续的 0，但 "::" 只能出现一次。（但是测试集好像不需要匹配这个条件）

所以我们知道ipv6的地址其实是xxxx:xxxx...

x = [0-9a-fA-F]（或者空）

即：

`([0-9a-fA-F]{1,4})$` 匹配IPv6地址中的最后一个段，可以是包含1到4位的十六进制数字或字母。

`^([0-9a-fA-F]{1,4}\:){7}` 匹配IPv6地址中的前7个段，每个段后面有一个冒号，表示一个IPv6地址有8个段。



所以最终的正则结果应该为：

*** ^([0-9a-fA-F]{1,4}\:){7}([0-9a-fA-F]{1,4})$***

所以添加代码为：
![image-20230930212556245](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930212556245.png)

重新运行代码
![image-20230930212618914](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20230930212618914.png)

通过所有测试！此实验结束！