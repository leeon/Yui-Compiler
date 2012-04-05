#include<stdio.h>
#include<stdlib.h>
#include "ylib.h"
#include "state.h"

/*
Yui.c
@author octobershiner
@version 0.1
2011 12 02
Description:  The main entry of the Yui Compiler.
*/

struct Node * Table;  /*符号表入口*/
struct Node * Token;   /*token序列*/

struct TreeNode * SyntaxTree; /*语法分析树*/

struct CodeNode * Code; /*中间代码*/

int Token_Error = NO;  
int Syntax_Error = NO;
int Sem_Error = NO;

/*主函数入口*/
int main ( int argc , char * argv[] )
{
    hello();
    Table = initTable(Table);
    Code = initCode(Code);

	scanner(argv[1]);
	writeTokenFile(Token);
	parser();
		
	if(argc>2&&strcmp(argv[2],"-f")==0)
	{
		printToken(Table);
	}
	else if(argc>2&&strcmp(argv[2],"-g")==0)
	{
		graphviz();
	}
	else if(argc>2&&strcmp(argv[2],"-p")==0)
	{
		printCode(Code);
	}
	writeCode(Code);
	/*
	initStack();
	push(1);
	push(2);
	push(3);
	pop();
	if(searchStack(1)==YES)
	{
		printf("find it\n");
	}
	*/
	return 0;
}