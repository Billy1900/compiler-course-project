# Intro

- [Tiny C compiler](https://github.com/o0olele/tiny-c-compiler)：using Javascript, maybe not useful
- [8cc C Compiler](https://github.com/rui314/8cc): too complex
- [9cc C compiler](https://github.com/rui314/9cc): too complex
- [chibicc](https://github.com/rui314/chibicc): too complex
- [Introduction to C compiler creation for people who want to know the low layer](https://www.sigbus.info/compilerbook):standford Book
- [SmallerC](https://github.com/alexfru/SmallerC): too complex
- [cCompiler](https://github.com/Uyouii/cCompiler): too complex

# How to make it work?

```
flex lexer.l//生成lexer.yy.c
bison -d -v parser.y//生成parser.tab.h, parser.tab.c 
gcc display.c semantic_analysis.c parser.tab.c lex.yy.c -lfl -o test3
./test3 test3.c
```

# Issue
test3.c 可能会报错: unexpected TYPE. 

test3.c是fact程序，官方要求的是fibo程序，由于时间久远，要花很多时间去检查建立的语法树，所以建议测试案例换成官方的fibo，这样应该不会报错
