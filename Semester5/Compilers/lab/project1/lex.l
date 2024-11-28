%{
    #include "Node.hpp"
    #include "string.h"
    #include "syntax.tab.h"
    //使用Bison的全局变量，并在每次匹配词时进行操作，记录，用于定位代码中出现问题的位置
    #define YY_USER_ACTION \
        yylloc.first_line = yylineno; \
        yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng;\
        yycolumn += yyleng;
    //定义一些全局变量
    int yycolumn = 1;//用于记录标记位置
%}

%option yylineno 
%option noyywrap 
/* 
%option yylineno //记录行号
%option noyywrap //不使用自带的yywrap函数
digit   [0-9]  //数字
hex (0|[1-9a-fA-F][0-9a-fA-F]*) //十六进制数
decimal_unit {digit}+ //十进制数
hexadecimal_uint 0(x|X){hex} //十六进制整数
int {decimal_unit}|{hexadecimal_uint} //整数
normal_char (\').(\') //普通字符不包括16进制的
hex_char (\')(\\)(x|X){hex}(\') //16进制字符
char {normal_char}|{hex_char} //字符
float  (0|[1-9][0-9]*)(\.){digit}+ //浮点数
letter_ [a-zA-Z_] //字母+下划线
id  {letter_}({letter_}|{digit})* //标识符
escape_char [ \t\n\r] //转义字符
ws {escape_char}+ //匹配多个转义字符

*/
digit   [0-9]  
hex (0|[1-9a-fA-F][0-9a-fA-F]*)
decimal_unit {digit}+
hexadecimal_uint 0(x|X){hex}
int {decimal_unit}|{hexadecimal_uint}
normal_char (\').(\')
hex_char (\')(\\)(x|X){hex}(\')
char {normal_char}|{hex_char}
float  (0|[1-9][0-9]*)(\.){digit}+
letter_ [a-zA-Z_] 
id  {letter_}({letter_}|{digit})*
escape_char [ \t\n\r]
ws {escape_char}+

%%




{ws} {
      /* 不进行操作 */
}//不进行操作
"int"|"float"|"char" {
    yylval.node = new Node(TYPE::TYPE,"TYPE",yytext,yylineno); 
    return TYPE;
}
"struct" {
    yylval.node = new Node(TYPE::OTHER,"STRUCT","struct",yylineno);
    return STRUCT;
}
"if" {
    yylval.node = new Node(TYPE::OTHER,"IF","if",yylineno);
    return IF;
}
"else" {
    yylval.node = new Node(TYPE::OTHER,"ELSE","else",yylineno);
    return ELSE;
}
"while" {
    yylval.node = new Node(TYPE::OTHER,"WHILE","while",yylineno);
    return WHILE;
}
"return" {
    yylval.node = new Node(TYPE::OTHER,"RETURN","return",yylineno);
    return  RETURN;
}
"." {
    yylval.node = new Node(TYPE::OTHER,"DOT",".",yylineno);
    return DOT;
}
";" {
    yylval.node = new Node(TYPE::OTHER,"SEMI",";",yylineno);
    return SEMI;
}
"," {
    yylval.node = new Node(TYPE::OTHER,"COMMA",",",yylineno);
    return COMMA;
}
"=" {
    yylval.node = new Node(TYPE::OTHER,"ASSIGN","=",yylineno);
    return ASSIGN;
}

"<" {
    yylval.node = new Node(TYPE::OTHER,"LT","<",yylineno);
     return LT;
}
"<=" {
    yylval.node = new Node(TYPE::OTHER,"LE","<=",yylineno);
     return LE;
}
">" {
    yylval.node = new Node(TYPE::OTHER,"GT",">",yylineno); 
    return GT;
}
">=" {
    yylval.node = new Node(TYPE::OTHER,"GE",">=",yylineno); 
    return GE;
}
"!=" {
    yylval.node = new Node(TYPE::OTHER,"NE","!=",yylineno); 
    return NE;
}
"==" {
    yylval.node = new Node(TYPE::OTHER,"EQ","==",yylineno); 
    return EQ;
}
"+" {
    yylval.node = new Node(TYPE::OTHER,"PLUS","+",yylineno);
    return PLUS;
}
"-" {
    yylval.node = new Node(TYPE::OTHER,"MINUS","-",yylineno);
    return MINUS;
}
"*" {
    yylval.node = new Node(TYPE::OTHER,"MUL","*",yylineno);
    return MUL;
}
"/" {
    yylval.node = new Node(TYPE::OTHER,"DIV","/",yylineno);
    return DIV;
}     
"&&" {
    yylval.node = new Node(TYPE::OTHER,"AND","&&",yylineno);
    return AND;
}
"||" {
    yylval.node = new Node(TYPE::OTHER,"OR","||",yylineno);
    return OR;
}
"!" {
    yylval.node = new Node(TYPE::OTHER,"NOT","!",yylineno);
    return NOT;
}     
"(" {
    yylval.node = new Node(TYPE::OTHER,"LP","(",yylineno);
    return  LP;
}        
")" {
    yylval.node = new Node(TYPE::OTHER,"RP",")",yylineno);
    return RP;
}
"[" {
    yylval.node = new Node(TYPE::OTHER,"LB","[",yylineno);
    return LB;
}
"]" {
    yylval.node = new Node(TYPE::OTHER,"RB","]",yylineno);
    return RB;
}
"{" {
    yylval.node = new Node(TYPE::OTHER,"LC","{",yylineno);
    return LC;
}
"}" {
    yylval.node = new Node(TYPE::OTHER,"RC","}",yylineno);
    return RC;
}
"\"" {
    
}

{int} {
    yylval.node= new Node(TYPE::INT,"INT",yytext,yylineno);
     return INT;
}
{float}  {
    yylval.node = new Node(TYPE::FLOAT,"FLOAT",yytext,yylineno);
    return FLOAT;
}

{hex_char}  {
    if ((int)strlen(yytext) > 6) {//处理超过6位的十六进制数
        printf("Error type A at Line %d: unknown lexem111e %s\n",yylineno,yytext);
    return UNKNOWN;
    }else { //处理小于6位的十六进制数
        yylval.node = new Node(TYPE::CHAR,"CHAR",yytext,yylineno); return CHAR;
        };
} 
{char}  {
    yylval.node = new Node(TYPE::CHAR,"CHAR",yytext,yylineno);
    return CHAR;
}
{id}    {
    yylval.node = new Node(TYPE::ID,"ID",yytext,yylineno);
    return ID;
}

{int}+{id}      {
    printf("Error type A at Line %d: unknown lexeme %s\n",yylineno,yytext);
}
{char}+{id}     {
    printf("Error type A at Line %d: unknown lexeme %s\n",yylineno,yytext);
}
{float}+{id} {
    printf("Error type A at Line %d: unknown lexeme %s\n",yylineno,yytext);
}
{id}{char}+{id}* {
    printf("Error type A at Line %d: unknown lexeme %s\n",yylineno,yytext);
}
{id}{float}+{id}* {
    printf("Error type A at Line %d: unknown lexeme %s\n",yylineno,yytext);
}

"//" {
    /*可选功能 识别注释*/
    // 单行注释，忽略直到换行符
    char c;
    do {
        c = yyinput();
    } while (c != '\n');
    unput(c);
}   
"/*" {
    // 多行注释，忽略直到遇到 "*/"
    char c;
    do {
        c = yyinput();
    } while (c != '*' || yyinput() != '/');
}

(\').*(\') {
    printf("Error type A at Line %d: unknown lexeme11 %s\n",yylineno,yytext);
    return UNKNOWN;
}//处理不合法的字符
. {
    printf("Error type A at Line %d: unknown lexeme %s\n",yylineno,yytext);
    return UNKNOWN;
}//处理不合法的任一符号

%%



