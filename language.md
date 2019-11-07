G[program]:   

program→ExtDefList        //program文件开始符号 ExtDefList指程序语句列表                                    
ExtDefList→ExtDef  ExtDefList | ε   //右递归， ExtDef指某一行的语句 ExtDefList指后面多行语句
ExtDef→Specifier  ExtDecList ; |Specifier  FunDec CompSt    //一行语句ExtDef中可能出现 1)标识符Specifier 变量声明列表ExtDecList  2)标识符Specifier 函数FunDec 函数体CompSt                                              
Specifier→int | float       //标识符Specifier 1)int  2)float     
ExtDecList→VarDec | VarDec , ExtDecList     //变量声明列表ExtDecList 1)变量  2）变量, 多个变量     
VarDec→ID   
FunDec→ID ( VarList )  | ID ( )    //函数声明FunDec 1）函数名(变量列表VarList) 2)函数名()参数空
VarList→ParamDec , VarList   |   ParamDec      //变量列表 参数声明ParamDec，(多个变量列表)
ParamDec→Specifier VarDec                //参数声明 标识符Specifier 变量声明VarDec
CompSt→{ DefList  StmList }              //函数体{定义列表，内部特定符号识别(while,if-else,return,';')}
StmList→Stmt  StmList | ε          
Stmt→Exp ;  |  CompSt  | return Exp ; | if ( Exp ) Stmt  | if ( Exp ) Stmt else Stmt   | while ( Exp ) Stmt     
DefList→Def DefList  |    ε           
Def→Specifier DecList ;               
DecList→Dec  | Dec , DecList        
Dec→VarDec  |  VarDec = Exp       
 
Exp →Exp =Exp  | Exp && Exp |  Exp || Exp   | Exp < Exp | Exp <= Exp    | Exp == Exp | Exp != Exp | Exp > Exp | Exp >= Exp   | Exp + Exp | Exp - Exp  | Exp * Exp | Exp / Exp | ID | INT | FLOAT   | ( Exp )  | - Exp  |  ! Exp  | ID ( Args )  | ID ( )   Args→Exp , Args  | Exp 
