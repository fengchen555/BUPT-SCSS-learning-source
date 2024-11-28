# BUPT Compiler Lab3 2023Fall










![image-20231026200749734](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231026200749734.png)

按照ppt要求完成，以下步骤。但是我怎么感觉是要通过calc_test.py测试，然后我就模仿一下parentheses

，改一下makefile。

![image-20231026200941928](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231026200941928.png)

这样就能生成 libcalc.so。然后测试一下。

![image-20231026201021547](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231026201021547.png)

就通过了python代码测试。好吧 也许上面的echo ...是用一下计算器。。

## Bison Exercise

**编写以下代码m 初始化result = 1; // 初始化为1，假设匹配成功，对Sting进行语法解析，若解析失败则打印出解析失败原因，并将result 置为0。**

```c
%{
    #include "lex.yy.c"
    int result;//定义变量
    void yyerror(const char *s) {
        fprintf(stderr, "Syntax error: %s\n", s);
        result = 0;
    }
    int result = 1; // 初始化为1，假设匹配成功


%}

%token LP RP LB RB LC RC

%%
String: 
    %empty
    |String LP String RP
    |String LB String RB
    |String LC String RC
%%

int validParentheses(char *expr){
    yy_scan_string(expr);
    yyparse();
    return result;
}



```









![image-20231027014521869](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231027014521869.png)

然后就可以看到对所有的样例测试通过，由于我对错误语法解析进行了输出，所以我们也可以看到这个为什么语法解析失败。我们对测试样例输出一下，看看具体情况。

![image-20231027014800334](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231027014800334.png)

![image-20231027014819384](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231027014819384.png)

![image-20231027014827980](https://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231027014827980.png)

然后可以看到样例跟我们的解析结果是一样。说明我们的解析没有问题，成功了！

总结：题目不是很难，但是理解题目，知道从哪去下手很困难，提前了解一下yacc会更方便些，能更快写出来。可以多写点注释，说一下要干什么，我们可能应该更明白些。