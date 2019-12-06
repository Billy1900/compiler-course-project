#include<stdio.h>
#include<stdbool.h>
#include "def.h"

//创建符号表
struct symboltable* Generate_symboltable(){
    struct symboltable *tempsymboltable = (struct symboltable*)malloc(sizeof(struct symboltable));
    tempsymboltable->index = 0;
    return tempsymboltable;
}

//插入表项
void Insert_symboltable(struct symboltable* STable,struct symbol* temp_symbol){
    STable->symbols[STable->index] = *temp_symbol;
    STable->index++;
}

//查询表,若查询到了，返回true；若没有，则返回false
//查询功能还不够
bool Search_symboltable(struct symboltable* STable,char* Wait_name){
    int index = STable->index;
    while (true){
        if (!strcmp(STable->symbols[index].name,Wait_name))       
            return true;
        if (index == 0)
            return false;
        index--;      
    }
}


int LEV = 0;
extern struct symboltable new_symboltable;
extern struct symbol_scope_TX new_scope_TX;
extern struct symbol new_symbol;

void semantic_Analysis(struct node *T){
    if(T){
        switch (T->kind){
          case EXT_DEF_LIST:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case EXT_VAR_DEF:
            T->ptr[1]->type = T->ptr[0]->type;
            semantic_Analysis(T->ptr[1]);
            break;
          case FUNC_DEF:
            T->ptr[1]->type = T->ptr[0]->type;
            semantic_Analysis(T->ptr[1]);
            semantic_Analysis(T->ptr[2]);
            //退栈symbol_scope_TX
            LEV--;
            break;
          case FUNC_DEC:
            LEV++;
            //建立symbol_scope_TX全局栈
            if(T->ptr[0]){
                T->ptr[0]->type = T->type;
                semantic_Analysis(T->ptr[0]);
            }
            break;
          case PARAM_LIST:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case PARAM_DEC:
            T->ptr[1]->type = T->ptr[0]->type;
            semantic_Analysis(T->ptr[1]);
            break;
          case COMP_STM:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case DEF_LIST:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case VAR_DEF:
            T->ptr[1]->type = T->ptr[0]->type;
            semantic_Analysis(T->ptr[1]);
            break;
          case DEC_LIST:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case ID:
            //装进scope_TX表中
            break;
          case ASSIGNOP:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break; 
          case ARRAY_DEF:
            T->ptr[1]->type = T->ptr[0]->type;
            semantic_Analysis(T->ptr[1]);
            break;
          case STM_LIST:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case EXP_STMT:
            semantic_Analysis(T->ptr[0]);
            break;
          case RETURN:
            semantic_Analysis(T->ptr[0]);
            break;
          case IF_THEN:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case IF_THEN_ELSE:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            semantic_Analysis(T->ptr[2]);
            break;
          case WHILE:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case FOR:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case EXT_DEC_LIST:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case AND: case OR: case RELOP: case PLUS: case MINUS: case STAR: case DIV: case COMADD: case COMSUB:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
          case UMINUS: case NOT: case AUTOADD_L: case AUTOADD_R: case AUTOSUB_L: case AUTOSUB_R: case FUNC_CALL:
            semantic_Analysis(T->ptr[0]);
            break; 
          case INT: case FLOAT:case CHAR:
            break;
          case ARGS:
            semantic_Analysis(T->ptr[0]);
            semantic_Analysis(T->ptr[1]);
            break;
        }
    }
}