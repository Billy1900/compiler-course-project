# Link
- [leeonfield/Semantic-analysis](https://github.com/leeonfield/Semantic-analysis/blob/master/TESTparse.cpp)
- [现代编译原理－－第四章（语义分析以及源码）](https://www.cnblogs.com/BlackWalnut/p/4527845.html)
- [编译原理实验报告：语义分析及中间代码生成](https://blog.csdn.net/Flamewaker/article/details/82903792)
 # How to make it work?
``` 
flex lexer.l//生成lexer.yy.c
bison -d -v parser.y//生成parser.tab.h, parser.tab.c 
gcc display.c parser.tab.c lex.yy.c -lfl -o test2
./test2 test2.c
```
