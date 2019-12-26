#include "def.h"

struct node *mknode(int kind, struct node *first, struct node *second, struct node *third, int pos)
{
    struct node *T = (struct node *)malloc(sizeof(struct node));
    T->kind = kind;
    T->ptr[0] = first;
    T->ptr[1] = second;
    T->ptr[2] = third;
    T->pos = pos;
    return T;
}

//对抽象语法树的先根遍历
void display(struct node *T, int indent)
{
    int i = 1;
    struct node *T0;

    if (T)
    {
        switch (T->kind)
        {
        case EXT_DEF_LIST:
            display(T->ptr[0], indent); //显示该外部定义列表中的第一个
            display(T->ptr[1], indent); //显示该外部定义列表中的其它外部定义
            break;
        case EXT_VAR_DEF:
            printf("%*c外部变量定义：\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示外部变量类型
            printf("%*c变量名：\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示变量列表
            break;
        case TYPE:
            printf("%*c类型： %s\n", indent, ' ', T->type_id);
            break;
        case EXT_DEC_LIST:
            display(T->ptr[0], indent); //依次显示外部变量名，
            display(T->ptr[1], indent); //后续还有相同的，仅显示语法树此处理代码可以和类似代码合并
            break;
        case FUNC_DEF:
            printf("%*c函数定义：\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示函数返回类型
            display(T->ptr[1], indent + 3); //显示函数名和参数
            display(T->ptr[2], indent + 3); //显示函数体
            break;
        case FUNC_DEC:
            printf("%*c函数名：%s\n", indent, ' ', T->type_id);
            if (T->ptr[0])
            {
                printf("%*c函数形参：\n", indent, ' ');
                display(T->ptr[0], indent + 3); //显示函数参数列表
            }
            else
                printf("%*c无参函数\n", indent + 3, ' ');
            break;
        case STRUCT_DEC:
        case STRUCT_DEF:
        case STRUCT_TAG:
        case EXP_ELE:
        case EXP_ARRAY:
            break;
        case PARAM_LIST:
            display(T->ptr[0], indent); //依次显示全部参数类型和名称，
            display(T->ptr[1], indent);
            break;
        case PARAM_DEC:
            if (T->ptr[0]->type == INT)
                printf("%*c类型：%s,参数名：%s\n", indent, ' ', "int", T->ptr[1]->type_id);
            else if (T->ptr[0]->type == FLOAT)
                printf("%*c类型：%s,参数名：%s\n", indent, ' ', "float", T->ptr[1]->type_id);
            else if (T->ptr[0]->type == CHAR)
                printf("%*c类型：%s,参数名：%s\n", indent, ' ', "char", T->ptr[1]->type_id);
            else if (T->ptr[0]->type == STRING)
                printf("%*c类型：%s,参数名：%s\n", indent, ' ', "string", T->ptr[1]->type_id);
            else
                printf("%*c类型：%s,参数名：%s\n", indent, ' ', "unknown", T->ptr[1]->type_id);
            break;
        case EXP_STMT:
            printf("%*c表达式语句：\n", indent, ' ');
            display(T->ptr[0], indent + 3);
            break;
        case RETURN:
            printf("%*c返回语句：\n", indent, ' ');
            display(T->ptr[0], indent + 3);
            break;
        case COMP_STM:
            printf("%*c复合语句：\n", indent, ' ');
            printf("%*c复合语句的变量定义：\n", indent + 3, ' ');
            display(T->ptr[0], indent + 6); //显示定义部分
            printf("%*c复合语句的语句部分：\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示语句部分
            break;
        case STM_LIST:
            display(T->ptr[0], indent); //显示第一条语句
            display(T->ptr[1], indent); //显示剩下语句
            break;
        case WHILE:
            printf("%*cWHILE循环语句：\n", indent, ' ');
            printf("%*cWHILE循环条件：\n", indent + 3, ' ');
            display(T->ptr[0], indent + 6); //显示循环条件
            printf("%*cWHILE循环体：\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示循环体
            break;
        case FOR:
            printf("%*cFOR循环语句：\n", indent, ' ');
            printf("%*cFOR循环条件：\n", indent + 3, ' ');
            display(T->ptr[0], indent + 6); //显示循环条件
            printf("%*cFOR循环体：\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示循环体
            break;
        case FOR_DEC:
            display(T->ptr[0], indent + 6);
            display(T->ptr[1], indent + 6);
            display(T->ptr[2], indent + 6);
            break;
        case IF_THEN:
            printf("%*c条件语句(IF_THEN)：\n", indent, ' ');
            printf("%*c条件：\n", indent + 3, ' ');
            display(T->ptr[0], indent + 6); //显示条件
            printf("%*cIF子句：\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示if子句
            break;
        case IF_THEN_ELSE:
            printf("%*c条件语句(IF_THEN_ELSE)：\n", indent, ' ');
            printf("%*c条件：\n", indent + 3, ' ');
            display(T->ptr[0], indent + 6); //显示条件
            printf("%*cIF子句：\n", indent + 3, ' ');
            display(T->ptr[1], indent + 6); //显示if子句
            printf("%*cELSE子句：\n", indent + 3, ' ');
            display(T->ptr[2], indent + 6); //显示else子句
            break;
        case DEF_LIST:
            display(T->ptr[0], indent); //显示该局部变量定义列表中的第一个
            display(T->ptr[1], indent); //显示其它局部变量定义
            break;
        case VAR_DEF:
            printf("%*cLOCAL VAR_NAME：\n", indent, ' ');
            display(T->ptr[0], indent + 3); //显示变量类型
            display(T->ptr[1], indent + 3); //显示该定义的全部变量名
            break;
        case DEC_LIST:
            printf("%*cVAR_NAME：\n", indent, ' ');
            T0 = T;
            while (T0)
            {
                if (T0->ptr[0]->kind == ID)
                    printf("%*c %s\n", indent + 3, ' ', T0->ptr[0]->type_id);
                else if (T0->ptr[0]->kind == ASSIGNOP)
                {
                    printf("%*c %s ASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
                    //显示初始化表达式
                    display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
                }
                else if (T0->ptr[0]->kind == PLUSASSIGNOP)
                {
                    printf("%*c %s PLUSASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
                    //显示初始化表达式
                    display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
                }
                else if (T0->ptr[0]->kind == MINUSASSIGNOP)
                {
                    printf("%*c %s MINUSASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
                    //显示初始化表达式
                    display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
                }
                else if (T0->ptr[0]->kind == STARASSIGNOP)
                {
                    printf("%*c %s STARASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
                    //显示初始化表达式
                    display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
                }
                else if (T0->ptr[0]->kind == DIVASSIGNOP)
                {
                    printf("%*c %s DIVASSIGNOP\n ", indent + 3, ' ', T0->ptr[0]->ptr[0]->type_id);
                    //显示初始化表达式
                    display(T0->ptr[0]->ptr[1], indent + strlen(T0->ptr[0]->ptr[0]->type_id) + 4);
                }
                T0 = T0->ptr[1];
            }
            break;
        case ID:
            printf("%*cID： %s\n", indent, ' ', T->type_id);
            break;
        case INT:
            printf("%*cINT：%d\n", indent, ' ', T->type_int);
            break;
        case FLOAT:
            printf("%*cFLAOT：%f\n", indent, ' ', T->type_float);
            break;
        case CHAR:
            printf("%*cCHAR：%s\n", indent, ' ', T->type_char);
            break;
        case STRING:
            printf("%*cSTRING：%s\n", indent, ' ', T->type_string);
            break;
        case ASSIGNOP:
        case MINUSASSIGNOP:
        case PLUSASSIGNOP:
        case STARASSIGNOP:
        case DIVASSIGNOP:
        case AND:
        case OR:
        case RELOP:
        case PLUS:
        case MINUS:
        case STAR:
        case DIV:
            printf("%*c%s\n", indent, ' ', T->type_id);
            display(T->ptr[0], indent + 3);
            display(T->ptr[1], indent + 3);
            break;
        case MMINUS:
        case PPLUS:
            printf("%*c%s\n", indent, ' ', T->type_id);
            display(T->ptr[0], indent + 3);
            break;
        case NOT:
        case UMINUS:
            printf("%*c%s\n", indent, ' ', T->type_id);
            display(T->ptr[0], indent + 3);
            break;
        case FUNC_CALL:
            printf("%*c函数调用：\n", indent, ' ');
            printf("%*c函数名：%s\n", indent + 3, ' ', T->type_id);
            display(T->ptr[0], indent + 3);
            break;
        case ARGS:
            i = 1;
            while (T)
            { //ARGS表示实际参数表达式序列结点，其第一棵子树为其一个实际参数表达式，第二棵子树为剩下的。
                struct node *T0 = T->ptr[0];
                printf("%*c第%d个实际参数表达式：\n", indent, ' ', i++);
                display(T0, indent + 3);
                T = T->ptr[1];
            }
            //                    printf("%*c第%d个实际参数表达式：\n",indent,' ',i);
            //                  display(T,indent+3);
            printf("\n");
            break;
        }
    }
}
