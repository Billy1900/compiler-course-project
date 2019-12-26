%error-verbose
%locations
%{
#include "stdio.h"
#include "math.h"
#include "string.h"
#include "def.h"
extern int yylineno;
extern char *yytext;
extern FILE *yyin;
void yyerror(const char* fmt, ...);
void display(struct node *,int);
%}

%union {
    int    type_int;
    float  type_float;
    char type_char[3];
    char type_string[31];
    char   type_id[32];
    struct node *ptr;
};

//  %type 定义非终结符的语义值类型
%type  <ptr> program ExtDefList ExtDef Specifier StructSpecifier OptTag Tag ExtDecList FuncDec CompSt VarList VarDec ParamDec Stmt ForDec StmList DefList Def DecList Dec Exp Args

//% token 定义终结符的语义值类型
%token <type_int> INT              //指定INT的语义值是type_int，有词法分析得到的数值
%token <type_id> ID RELOP TYPE  //指定ID,RELOP 的语义值是type_id，有词法分析得到的标识符字符串
%token <type_float> FLOAT         //指定ID的语义值是type_id，有词法分析得到的标识符字符串
%token <type_char> CHAR         //指定ID的语义值是type_id，有词法分析得到的标识符字符串
%token <type_string> STRING         //指定ID的语义值是type_id，有词法分析得到的标识符字符串

%token STRUCT LP RP LB RB LC RC SEMI COMMA DOT  //用bison对该文件编译时，带参数-d，生成的exp.tab.h中给这些单词进行编码，可在lex.l中包含parser.tab.h使用这些单词种类码
%token PPLUS MMINUS PLUS MINUS STAR DIV ASSIGNOP MINUSASSIGNOP PLUSASSIGNOP DIVASSIGNOP STARASSIGNOP AND OR NOT IF ELSE WHILE FOR RETURN

%right ASSIGNOP MINUSASSIGNOP PLUSASSIGNOP DIVASSIGNOP STARASSIGNOP // 赋值
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right UMINUS NOT PPLUS MMINUS // 负号和非
%right LB
%left RB
%left DOT
%nonassoc LOWER_THEN_ELSE
%nonassoc ELSE

%%

program: ExtDefList    {display($1,0);DisplaySymbolTable($1);}     /*显示语法树,语义分析 */;
         ;
ExtDefList: {$$=NULL;}
          | ExtDef ExtDefList {$$=mknode(EXT_DEF_LIST,$1,$2,NULL,yylineno);}   //每一个EXTDEFLIST的结点，其第1棵子树对应一个外部变量声明或函数
          ;
ExtDef:   Specifier ExtDecList SEMI   {$$=mknode(EXT_VAR_DEF,$1,$2,NULL,yylineno);}   //该结点对应一个外部变量声明
         | Specifier SEMI
         | Specifier FuncDec CompSt    {$$=mknode(FUNC_DEF,$1,$2,$3,yylineno);}         //该结点对应一个函数定义
         | error SEMI   {$$=NULL;}
         ;

ExtDecList:  VarDec      {$$=$1;}       /*每一个EXT_DECLIST的结点，其第一棵子树对应一个变量名(ID类型的结点),第二棵子树对应剩下的外部变量名*/
           | VarDec COMMA ExtDecList {$$=mknode(EXT_DEC_LIST,$1,$3,NULL,yylineno);}
           ;
/*
Specifier:  TYPE    {$$=mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);$$->type=(!strcmp($1,"int"))?INT:(!strcmp($1,"float"))?FLOAT:(!strcmp($1,"char"))?CHAR:(!strcmp($1,"string"))?STRING:NULL;}
          | StructSpecifier {}
           ;
*/
Specifier:  TYPE    {$$=mknode(TYPE,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);if(!strcmp($1, "int"))$$->type=INT;if(!strcmp($1, "float"))$$->type=FLOAT;if(!strcmp($1, "char"))$$->type=CHAR;if(!strcmp($1, "string"))$$->type=STRING;}
           | StructSpecifier {$$=$1;}
           ;

StructSpecifier: STRUCT OptTag LC DefList RC {$$=mknode(STRUCT_DEF,$2,$4,NULL,yylineno);}
          | STRUCT Tag  {$$=mknode(STRUCT_DEC,$2,NULL,NULL,yylineno);}
          ;

OptTag: {$$=NULL;}
       | ID {$$=mknode(STRUCT_TAG,NULL,NULL,NULL,yylineno);strcpy($$->struct_name,$1);}
       ;
Tag: ID {$$=mknode(STRUCT_TAG,NULL,NULL,NULL,yylineno);strcpy($$->struct_name,$1);}
    ;

VarDec:  ID          {$$=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}   //ID结点，标识符符号串存放结点的type_id
        | VarDec LB INT RB {struct node *temp=mknode(INT,NULL,NULL,NULL,yylineno);temp->type_int=$3;$$=mknode(ARRAY_DEC, $1, temp, NULL,yylineno);}//$$=mknode(ARRAY_DEC,$1,$3,NULL,yylineno);
         ;

FuncDec: ID LP VarList RP   {$$=mknode(FUNC_DEC,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
	|ID LP  RP   {$$=mknode(FUNC_DEC,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}//函数名存放在$$->type_id
        ;

VarList: ParamDec  {$$=mknode(PARAM_LIST,$1,NULL,NULL,yylineno);}
        | ParamDec COMMA  VarList  {$$=mknode(PARAM_LIST,$1,$3,NULL,yylineno);}
        ;

ParamDec: Specifier VarDec         {$$=mknode(PARAM_DEC,$1,$2,NULL,yylineno);}
         ;

// 定义部分和执行部分
CompSt: LC DefList StmList RC    {$$=mknode(COMP_STM,$2,$3,NULL,yylineno);}
       ;
StmList: {$$=NULL; }
        | Stmt StmList  {$$=mknode(STM_LIST,$1,$2,NULL,yylineno);}
        ;
Stmt:   Exp SEMI    {$$=mknode(EXP_STMT,$1,NULL,NULL,yylineno);}
      | CompSt      {$$=$1;}      //复合语句结点直接最为语句结点，不再生成新的结点
      | RETURN Exp SEMI   {$$=mknode(RETURN,$2,NULL,NULL,yylineno);}
      | IF LP Exp RP Stmt %prec LOWER_THEN_ELSE   {$$=mknode(IF_THEN,$3,$5,NULL,yylineno);}
      | IF LP Exp RP Stmt ELSE Stmt   {$$=mknode(IF_THEN_ELSE,$3,$5,$7,yylineno);}
      | WHILE LP Exp RP Stmt {$$=mknode(WHILE,$3,$5,NULL,yylineno);}
      | FOR LP ForDec RP Stmt {$$=mknode(FOR,$3,$5,NULL,yylineno);}
      ;

ForDec: Exp SEMI Exp SEMI Exp {$$=mknode(FOR_DEC,$1,$3,$5,yylineno);}
       | SEMI Exp SEMI {$$=mknode(FOR_DEC,NULL,$2,NULL,yylineno);}
       ;

DefList: {$$=NULL; }
        | Def DefList {$$=mknode(DEF_LIST,$1,$2,NULL,yylineno);}
        ;
Def:    Specifier DecList SEMI {$$=mknode(VAR_DEF,$1,$2,NULL,yylineno);}
        ;
DecList: Dec  {$$=mknode(DEC_LIST,$1,NULL,NULL,yylineno);}
       | Dec COMMA DecList  {$$=mknode(DEC_LIST,$1,$3,NULL,yylineno);}
	;
Dec:     VarDec  {$$=$1;}
       | VarDec ASSIGNOP Exp  {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}
       ;
Exp:    Exp ASSIGNOP Exp {$$=mknode(ASSIGNOP,$1,$3,NULL,yylineno);strcpy($$->type_id,"ASSIGNOP");}//$$结点type_id空置未用，正好存放运算符
      | Exp AND Exp   {$$=mknode(AND,$1,$3,NULL,yylineno);strcpy($$->type_id,"AND");}
      | Exp OR Exp    {$$=mknode(OR,$1,$3,NULL,yylineno);strcpy($$->type_id,"OR");}
      | Exp RELOP Exp {$$=mknode(RELOP,$1,$3,NULL,yylineno);strcpy($$->type_id,$2);}  //词法分析关系运算符号自身值保存在$2中
      | Exp PLUS Exp  {$$=mknode(PLUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"PLUS");}
      | Exp PLUS PLUS  {$$=mknode(PPLUS,$1,NULL,NULL,yylineno);strcpy($$->type_id,"PPLUS");}
      | Exp PLUS ASSIGNOP Exp {$$=mknode(PLUSASSIGNOP,$1,$4,NULL,yylineno);strcpy($$->type_id,"PLUSASSIGNOP");}
      | Exp MINUS Exp {$$=mknode(MINUS,$1,$3,NULL,yylineno);strcpy($$->type_id,"MINUS");}
      | Exp MINUS MINUS  {$$=mknode(MMINUS,$1,NULL,NULL,yylineno);strcpy($$->type_id,"MMINUS");}
      | Exp MINUS ASSIGNOP Exp {$$=mknode(MINUSASSIGNOP,$1,$4,NULL,yylineno);strcpy($$->type_id,"MINUSASSIGNOP");}
      | Exp STAR Exp  {$$=mknode(STAR,$1,$3,NULL,yylineno);strcpy($$->type_id,"STAR");}
      | Exp STAR ASSIGNOP Exp {$$=mknode(STARASSIGNOP,$1,$4,NULL,yylineno);strcpy($$->type_id,"STARASSIGNOP");}
      | Exp DIV Exp   {$$=mknode(DIV,$1,$3,NULL,yylineno);strcpy($$->type_id,"DIV");}
      | Exp DIV ASSIGNOP Exp {$$=mknode(DIVASSIGNOP,$1,$4,NULL,yylineno);strcpy($$->type_id,"DIVASSIGNOP");}
      | LP Exp RP     {$$=$2;}
      | MINUS Exp %prec UMINUS   {$$=mknode(UMINUS,$2,NULL,NULL,yylineno);strcpy($$->type_id,"UMINUS");}
      | NOT Exp       {$$=mknode(NOT,$2,NULL,NULL,yylineno);strcpy($$->type_id,"NOT");}
      | ID LP Args RP {$$=mknode(FUNC_CALL,$3,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
      | ID LP RP      {$$=mknode(FUNC_CALL,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
      | Exp LB Exp RB {$$=mknode(EXP_ARRAY,$1,$3,NULL,yylineno);}
      | Exp DOT ID {struct node *temp=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$3);$$=mknode(EXP_ELE,$1,temp,NULL,yylineno);}//$$=mknode(EXP_ELE,$1,$3,NULL,yylineno);
      | ID            {$$=mknode(ID,NULL,NULL,NULL,yylineno);strcpy($$->type_id,$1);}
      | INT           {$$=mknode(INT,NULL,NULL,NULL,yylineno);$$->type_int=$1;$$->type=INT;}
      | FLOAT         {$$=mknode(FLOAT,NULL,NULL,NULL,yylineno);$$->type_float=$1;$$->type=FLOAT;}
      | CHAR           {$$=mknode(CHAR,NULL,NULL,NULL,yylineno);strcpy(yylval.type_char,$1);$$->type=CHAR;}
      | STRING         {$$=mknode(STRING,NULL,NULL,NULL,yylineno);strcpy(yylval.type_string,$1);$$->type=STRING;}
      ;

Args:    Exp COMMA Args    {$$=mknode(ARGS,$1,$3,NULL,yylineno);}
       | Exp               {$$=mknode(ARGS,$1,NULL,NULL,yylineno);}
       ;

%%

int main(int argc, char *argv[]){
	yyin=fopen(argv[1],"r");
	if (!yyin) return 0;
	yylineno=1;
	yyparse();
	return 0;
	}

#include<stdarg.h>
void yyerror(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "===>ERROR:Grammar Error at Line %d Column %d: ", yylloc.first_line,yylloc.first_column);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, ".\n");
}
