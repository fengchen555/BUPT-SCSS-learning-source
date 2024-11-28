%{
    #include"lex.yy.c"
    void yyerror(const char*s);
    bool type_A_Error = 0;
    bool type_B_Error = 0;
    Node* Ast_Tree;//语法树的根节点，树根，整个树
%}
%locations  //用于生成位置信息

%union{
     Node* node;
} 
//用于当语法分析器在执行规约操作时将数据传递给语法树节点

// 声明TOKEN
%token <node> INT FLOAT CHAR ID TYPE STRUCT IF ELSE WHILE FOR RETURN
%token <node> DOT SEMI COMMA ASSIGN LT LE GT GE NE EQ  
%token <node> PLUS MINUS MUL DIV AND OR NOT LP RP LB RB LC RC //定义了运算符的优先级
%token <node> UNKNOWN

%type <node> Program ExtDefList
%type <node> ExtDef ExtDecList Specifier StructSpecifier VarDec
%type <node> FunDec VarList ParamDec CompSt StmtList Stmt DefList
%type <node> Def DecList Dec Args Exp


//定义优先级类别
%nonassoc UELSE// 定义了else的优先级,处理else时不会引起二义性
%nonassoc ELSE// 定义了else的优先级,处理else时不会引起二义性
//%nonassoc UMINUS// 定义了负号MINUS的优先级,处理负号时不会引起二义性
%right ASSIGN //对赋值语句赋予右结合
%left OR AND //对与和或赋予左结合
%left LT LE GT GE NE EQ //对比较运算符赋予左结合
%left PLUS MINUS //对加减运算符赋予左结合
%left MUL DIV //对乘除运算符赋予左结合
%right NOT //对非运算符赋予右结合
%left LP RP LB RB DOT LC RC   //指定了括号和点运算符的结合性和优先级

%%

/* high-level definition */ 
Program: ExtDefList {
    $$ = new Node(TYPE::MEDIAN,"Program","",@$.first_line);
    $$->addChild({$1});
    Ast_Tree = $$;
};
ExtDefList 
    : {$$=new Node(TYPE::NOTHING,"ExtDefList","",@$.first_line);}//这里的ExtDefList可以为空
    |ExtDef ExtDefList {
        $$=new Node(TYPE::MEDIAN,"ExtDefList","",@$.first_line); $$->addChild({$1,$2});
    }
;
ExtDef
    : error ExtDecList SEMI    {printf("Error type B at Line %d: Missing specifier\n",@$.first_line); type_B_Error=1;}//这里的error是为了处理缺少Specifier的情况
    | Specifier ExtDecList SEMI  {$$=new Node(TYPE::MEDIAN,"ExtDef","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Specifier SEMI             {$$=new Node(TYPE::MEDIAN,"ExtDef","",@$.first_line); $$->addChild({$1,$2});}
    | Specifier FunDec CompSt    {$$=new Node(TYPE::MEDIAN,"ExtDef","",@$.first_line); $$->addChild({$1,$2,$3});}
;
ExtDecList
    : VarDec  {$$=new Node(TYPE::MEDIAN,"ExtDecList","",@$.first_line); $$->addChild({$1});}
    | VarDec COMMA ExtDecList   {$$=new Node(TYPE::MEDIAN,"ExtDecList","",@$.first_line); $$->addChild({$1,$2,$3});}   
;
/* specifier */ 
Specifier
    : TYPE     {$$=new Node(TYPE::MEDIAN,"Specifier","",@$.first_line); $$->addChild({$1});}
    | StructSpecifier   {$$=new Node(TYPE::MEDIAN,"Specifier","",@$.first_line); $$->addChild({$1});}
;         
StructSpecifier
    : STRUCT ID LC DefList RC    {$$=new Node(TYPE::MEDIAN,"StructSpecifier","",@$.first_line); $$->addChild({$1,$2,$3,$4,$5});}
    | STRUCT ID     {$$=new Node(TYPE::MEDIAN,"StructSpecifier","",@$.first_line); $$->addChild({$1,$2});}
;
/* declarator */ 
VarDec
    : ID     {$$=new Node(TYPE::MEDIAN,"VarDec","",@$.first_line); $$->addChild({$1});}
    | VarDec LB INT RB  {$$=new Node(TYPE::MEDIAN,"VarDec","",@$.first_line); $$->addChild({$1,$2,$3,$4});}   
;
FunDec
    : ID LP error {$$=new Node(TYPE::MEDIAN,"FunDec","",@$.first_line); $$->addChild({$1,$2}); printf("Error type B at Line %d: Missing closing parenthesis ')'\n",@$.first_line); type_B_Error=1;}  
    | ID LP VarList RP                      {$$=new Node(TYPE::MEDIAN,"FunDec","",@$.first_line); $$->addChild({$1,$2,$3,$4});}
    | ID LP RP                                {$$=new Node(TYPE::MEDIAN,"FunDec","",@$.first_line); $$->addChild({$1,$2,$3});}
;
VarList
    : ParamDec {$$=new Node(TYPE::MEDIAN,"VarList","",@$.first_line); $$->addChild({$1});}
    | ParamDec COMMA VarList {$$=new Node(TYPE::MEDIAN,"VarList","",@$.first_line); $$->addChild({$1,$2,$3});}

   
    
;
ParamDec: Specifier VarDec {$$=new Node(TYPE::MEDIAN,"ParamDec","",@$.first_line); $$->addChild({$1,$2});}
;
/* statement */ 
CompSt: LC DefList StmtList RC {$$=new Node(TYPE::MEDIAN,"CompSt","",@$.first_line); $$->addChild({$1,$2,$3,$4});}
;

StmtList
    :               {$$=new Node(TYPE::NOTHING,"StmtList","",@$.first_line);}//这里的StmtList可以为空
    |Stmt StmtList  {$$=new Node(TYPE::MEDIAN,"StmtList","",@$.first_line); $$->addChild({$1,$2});}
;
Stmt
    : Exp SEMI                                {$$=new Node(TYPE::MEDIAN,"Stmt","",@$.first_line); $$->addChild({$1,$2});}
    | CompSt                                  {$$=new Node(TYPE::MEDIAN,"Stmt","",@$.first_line); $$->addChild({$1});}
    | RETURN Exp error                        {$$=new Node(TYPE::MEDIAN,"Stmt","",@$.first_line); $$->addChild({$1,$2}); printf("Error type B at Line %d: Missing semicolon ';'\n",@$.first_line); type_B_Error = 1;}
    | RETURN Exp SEMI                         {$$=new Node(TYPE::MEDIAN,"Stmt","",@$.first_line); $$->addChild({$1,$2,$3});}
    | IF LP Exp RP Stmt  %prec UELSE          {$$=new Node(TYPE::MEDIAN,"Stmt","",@$.first_line); $$->addChild({$1,$2,$3,$4,$5});}
    | IF LP Exp RP Stmt ELSE Stmt             {$$=new Node(TYPE::MEDIAN,"Stmt","",@$.first_line); $$->addChild({$1,$2,$3,$4,$5,$6,$7});}
    | WHILE LP Exp RP Stmt                    {$$=new Node(TYPE::MEDIAN,"Stmt","",@$.first_line); $$->addChild({$1,$2,$3,$4,$5});}
    | FOR LP Exp SEMI Exp SEMI Exp RP Stmt    {$$=new Node(TYPE::MEDIAN,"Stmt","",@$.first_line); $$->addChild({$1,$2,$3,$4,$5,$6,$7,$8,$9});}
; 
/* local definition */ 
DefList 
    :           {$$=new Node(TYPE::NOTHING,"DefList","",@$.first_line);}//这里的DefList可以为空
    | Def DefList   {$$=new Node(TYPE::MEDIAN,"DefList","",@$.first_line); $$->addChild({$1,$2});}       
;
Def 
    : Specifier DecList error   {$$=new Node(TYPE::MEDIAN,"Def","",@$.first_line); $$->addChild({$1,$2}); printf("Error type B at Line %d: Missing semicolon ';'\n",@$.first_line); type_B_Error = 1;}
    | Specifier DecList SEMI    {$$=new Node(TYPE::MEDIAN,"Def","",@$.first_line); $$->addChild({$1,$2,$3});}
;
DecList 
    :Dec    {$$=new Node(TYPE::MEDIAN,"DecList","",@$.first_line); $$->addChild({$1});}
    | Dec COMMA DecList {$$=new Node(TYPE::MEDIAN,"DecList","",@$.first_line); $$->addChild({$1,$2,$3});}
;
Dec 
    :  VarDec   {$$=new Node(TYPE::MEDIAN,"Dec","",@$.first_line); $$->addChild({$1});}
    | VarDec  ASSIGN  Exp   {$$=new Node(TYPE::MEDIAN,"Dec","",@$.first_line); $$->addChild({$1,$2,$3});}
;
/* Expression  */ 
Exp 
    : Exp ASSIGN Exp    {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Exp AND Exp       {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}   
    | Exp OR Exp        {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});} 
    | Exp LT Exp        {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});} 
    | Exp LE Exp        {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});} 
    | Exp GT Exp        {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Exp GE Exp        {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});} 
    | Exp NE Exp        {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Exp EQ Exp        {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Exp PLUS Exp      {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Exp MINUS Exp     {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Exp MUL Exp   {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Exp DIV Exp   {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | LP  Exp  RP   {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | MINUS Exp      {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2});}
    | NOT Exp           {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2});}  
    | ID  LP Args error {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3}); printf("Error type B at Line %d: Missing closing parenthesis ')'\n",@$.first_line); type_B_Error = 1;}
    | ID  LP Args RP    {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3,$4});}
    | ID LP RP          {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | Exp  LB  Exp  RB  {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3,$4});}
    | Exp DOT ID        {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1,$2,$3});}
    | ID   /*ID*/       {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1});}//这里多打了一个ID找了半天
    | INT               {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1});}
    | FLOAT             {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1});}
    | CHAR              {$$=new Node(TYPE::MEDIAN,"Exp","",@$.first_line); $$->addChild({$1});}
    | UNKNOWN           {type_A_Error = 1;}
;
Args 
    : Exp             {$$=new Node(TYPE::MEDIAN,"Args","",@$.first_line); $$->addChild({$1});}
    | Exp COMMA Args  {$$=new Node(TYPE::MEDIAN,"Args","",@$.first_line); $$->addChild({$1,$2,$3});}
;


%%
//下面的main自定义error处理函数
void yyerror(const char*s){
}

int main(int argc, char** argv){
    if(argc <= 1) {
        printf("Please input the file name!\n");
        return 1;
    }
    else{
        FILE* f = fopen(argv[1],"r");
        if(!f){
            printf("File open failed!\n");
            return 1;
        }
    
        yyrestart(f);
        //yyparse();
        if(!yyparse() && (type_A_Error == 0) &&(type_B_Error == 0)){
            // printf("Congratulations! No syntax error!\n");
            Node::print(Ast_Tree,0);
        }
        else{
            // printf("Sorry! There are syntax errors!\n");
        }
    }
    return 0;
}



