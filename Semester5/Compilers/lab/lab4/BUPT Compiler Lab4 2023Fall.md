# BUPT Compiler Lab4 2023Fall





在syntax注意到这个代码
![image-20231102192822367](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231102192822367.png)



我猜想就是要自己写些匹配错误的规则，然后就开始写。



```c
%{
    #include"lex.yy.c"
    void yyerror(const char*);
%}

%token LC RC LB RB COLON COMMA
%token STRING NUMBER
%token TRUE FALSE VNULL
%%

Json:
      Value
    ;
Value:
      Object
    | Array
    | STRING
    | NUMBER
    | TRUE
    | FALSE
    | VNULL
    ;
Object:
      LC RC
    | LC Members COMMA RC error{puts("Comma instead if closing brace, recovered");}
    | LC Members RC Value error{puts("Extra value after close, recovered");}
    | LC Members RC 

    ;
Members:
      Member
    | Member COMMA RC error{puts("Extra comma, recovered");}
    | Member COMMA error{puts("Comma instead if closing brace, recovered");}
    | Member COMMA Members

    ;
Member:
      STRING COLON COLON  error{puts("Double colon, recovered");}
      |STRING Value  error{puts("Missing colon, recovered");}
      |STRING COLON NUMBER NUMBER error{ puts("Numbers cannot have leading zeroes, recovered"); }
      |STRING COLON Value

      |STRING COMMA Value error{puts("Comma instead of colon, recovered");}
    ;
Array:
      LB RB
    | Array RB error { puts("Extra close, recovered"); }
    | LB Values RB COMMA error { puts("Comma after the close, recovered"); }
    | LB Values RB
    | LB Values RC error { puts("mismatch, recovered"); }
    | LB Values error{ puts("Unclosed array, recovered"); }
    | LB COMMA error {puts("<-- missing value, recovered");}

    ;
Values:
      Value
    | Value COLON Value error{puts("Colon instead of comma, recovered");}
    | Value COMMA RB error{ puts("extra comma, recovered"); }
    | Value COMMA COMMA error{ puts("double extra comma, recovered"); }

    | Value COMMA Values
    ;
%%

void yyerror(const char *s){
    printf("syntax error: ");
}

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();
    return 0;
}

```

就写一系列规则，一直写，写了挺久。然后看看结果。



![image-20231102193336560](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231102193336560.png)

![image-20231102193352325](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231102193352325.png)

![image-20231102193426216](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231102193426216.png)

![image-20231102193501991](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231102193501991.png)

![image-20231102193527914](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231102193527914.png)

成功实现了错误恢复，并且完成了实验的基本要求。复现了结果。

## 解决冲突：

使用命令：bison -t -d -v syntax.y查看具体是哪里出现冲突。

![image-20231105004510945](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231105004510945.png)

一个个更改就行。

先将token改成left

![image-20231105004606592](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231105004606592.png)

注意到其实是我写了个递归,改一下.

![image-20231105004542346](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231105004542346.png)

改成这样

![image-20231105004728377](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231105004728377.png)

然后可以看到只有一个从冲突了，应该就是优先级减少了一个，array的解决又一个。

![image-20231105004803699](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231105004803699.png)

注意看冲突的地方

![image-20231105004912849](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231105004912849.png)

![image-20231105005008159](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231105005008159.png)

注意看 **STRING COMMA Value error{puts("Comma instead of colon, recovered");}与 Member COMMA error{puts("Comma instead if closing brace, recovered");}**是重复的，删掉一个。

![image-20231105005250753](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/image-20231105005250753.png)

然后显示没有冲突。运行看看。

![屏幕截图2023-11-5](http://typora-image-fengchen.oss-cn-beijing.aliyuncs.com/img/屏幕截图2023-11-5.png)

源代码：

```c
%{
    #include"lex.yy.c"
    void yyerror(const char*);
%}
%right LC RC LB RB COLON COMMA
%token STRING NUMBER
%token TRUE FALSE VNULL
%%

Json:
      Value
    ;
Value:
      Object
    | Array
    | STRING
    | NUMBER
    | TRUE
    | FALSE
    | VNULL
    ;
Object:
      LC RC
    | LC Members RC 
    | LC Members RC Value error{puts("Extra value after close, recovered");}


    ;
Members:
      Member
    | Member COMMA Members
    | Member COMMA RC error{puts("Extra comma, recovered");}
    | Member COMMA error{puts("Comma instead if closing brace, recovered");}


    ;
Member:
    STRING COLON Value
    |STRING COLON NUMBER NUMBER error{ puts("Numbers cannot have leading zeroes, recovered"); }
    |STRING COLON COLON  error{puts("Double colon, recovered");}
    |STRING COMMA Value error{puts("Comma instead of colon, recovered");}
    |STRING Value  error{puts("Missing colon, recovered");}


    ;
Array:
      LB RB
    | LB Values RB
    | LB Values RB RB error { puts("Extra close, recovered"); }
    | LB Values RB COMMA error { puts("Comma after the close, recovered"); }

    | LB Values RC error { puts("mismatch, recovered"); }
    | LB Values error{ puts("Unclosed array, recovered"); }
    | LB COMMA error {puts("<-- missing value, recovered");}

    ;
Values:
      Value
    | Value COMMA Values
    | Value COLON Value error{puts("Colon instead of comma, recovered");}
    | Value COMMA RB error{ puts("extra comma, recovered"); }
    | Value COMMA COMMA error{ puts("double extra comma, recovered"); }
    
    ;
%%

void yyerror(const char *s){
    printf("syntax error: ");
}

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();
    return 0;
}

```

完成此实验，并通过测试。
