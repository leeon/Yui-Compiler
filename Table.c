#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "state.h"
#include "ylib.h"

/*
Table.c
@author octobershiner
@version 0.1
2011 12 02
Description:  This file defines the data structure of Token and implements the file accessing of token.
*/



/*
函数功能：初始化符号表
*/
struct Node* initTable(struct Node* h)
{

	int i;
	for (i = 1; i <= 30; i++)
	{
		h = add(h,symbol[i-1],i,0,0,0);
	}
	for(i = 40; i <= 48; i++)
	{
		h = add(h,keywords[i-40],i,0,0,0);
	}
	printf("Symbol Table initialized successfully.\n");
	
	
	return h;
}


/*
函数功能：添加新的节点
*/
struct Node* add(struct Node*h,char * p,int VALUE,int LINE,int BLOCK,int type)
{
    struct Node* newNode = NULL;
	struct Node* temp = h;
	
	
	newNode = (struct Node*)malloc(sizeof(struct Node));
	if(newNode == NULL)
	{
		printf("fail in creating symbol table .\n");
		exit(0);
	}
	
	/*初始化新的节点*/
	newNode->value = VALUE;
	newNode->line = LINE;
	newNode->block_num = BLOCK;
	newNode->type = type;
	strcpy(newNode->name,p);
	
	newNode->next =NULL;
	
	
	
	/*当链表为空的情况*/
	if(h ==NULL)
	h = newNode;
	else
	{
		while(temp->next != NULL)
		temp = temp->next;
		temp->next = newNode;
	}
	
	return h;
}


/*
函数功能：打印序列
*/
void printToken(struct Node* h)
{
	struct Node* p = h;
	printf("(Attribute , Value, Block)\n");
	
	while(p != NULL)
	{
		char* c = p->name;
		printf("( %d ,",p->value);
		while(*c != '\0')
		printf("%c",*c++);
		printf(" ,%d",p->block_num);
		printf(" ,%d",p->type);
		printf(")\n");
		
		p = p->next;
	}

}

/*
函数功能： 将token序列写入文件
*/

void writeTokenFile(struct Node* h)
{
     struct Node* p = h;
	 FILE * fp = NULL;
	 if ( ( fp = fopen("token.txt","w+") ) == NULL)
	 {
			printf("YuiToken->Error: Create file fail.\n");
			exit(0);
	 } 
	 
	while(p != NULL)
	{
		char* c = p->name;
		fprintf(fp,"( %d ,",p->value);
		while(*c != '\0')
		fprintf(fp,"%c",*c++);
		fprintf(fp," )\r\n");	
		p = p->next;
	}
	
}
