************************************************
YuiToken YuiSem

-------------------Info-----------------

OS: cygwin
Compiler: gcc
Language: C89

----------------------------------------

-----------------Structure--------------

//ylib.h
包含了全局变量的声明
提供了词法分析和语法分析的公共接口
提供符号表链表操作开放接口
提供语法树操作

//ylib.c
基本方法的实现

//Yui.c
主程序入口

//Table.c
实现符号表和token序列的数据额结构

//state.h
保存所有的词法分析状态

//Scanner.c
实现词法分析

//Syntax.c
实现语法分析树的数据结构
----------------------------------------

-----------------Error------------------

非法标识符
需要转义的字符
括号等特殊标识符匹配报错

----------------------------------------






待解决问题
----------------------------------------
1 暂不支持转移字符
2 引号匹配错误后，行提取停止
*************************************************