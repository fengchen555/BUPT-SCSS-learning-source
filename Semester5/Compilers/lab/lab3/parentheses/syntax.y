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


