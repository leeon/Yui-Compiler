#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "state.h"
#include "ylib.h"


/*block栈操作*/
int Stack[STACK_SIZE];
int *top = NULL;
int *bottom = NULL;

void initStack()
{
	top = Stack;
	bottom = Stack;
}

void push(int num)
{
	top ++;
	if(top == (bottom + STACK_SIZE))
	{
	   printf("STACK FULL!\n");
	   exit(1);
	}
	*top = num; 
}

int pop()
{
    int result = 0;
	if(top == bottom)
	{
		printf("STACK FULL!\n");
	}
	else
	{
		result = *top;
	    top--;
	}
	return result;
}

int searchStack(int num)
{
	int * current = top;
	while(current != bottom)
	{
		if(num == *current--)
			return YES;
	}		
	return NO;		
}

/***********************生成中间代码***************************/


struct CodeNode *initCode(struct CodeNode* h)
{
	char sentence[20] = "BEGIN";
	h = addCode(h,sentence,01,NEXT);
    printf("Intermediate code generation begins.\n");
	return h;
}
struct CodeNode* gencode(struct CodeNode* h,int addr,char* LEFT,char*OP,char* RIGHT)
{
	char sentence[200]="";
	char *blank = " ";

	strcat(sentence,LEFT);
	strcat(sentence,blank);
	strcat(sentence,OP);
	strcat(sentence,blank);
	strcat(sentence,RIGHT);
	

		
	h = addCode(h,sentence,addr,NEXT);
	return h;
}

struct CodeNode* emit(struct CodeNode* h,int addr,char*RESULT, char* OP1, char*LEFT, char* OP2,char*RIGHT)
{
	char sentence[200]="";
	char *blank = " ";
	strcat(sentence,RESULT);
	strcat(sentence,blank);
	strcat(sentence,OP1);
	strcat(sentence,blank);
	strcat(sentence,LEFT);
	strcat(sentence,blank);
	strcat(sentence,OP2);
	strcat(sentence,blank);
	strcat(sentence,RIGHT);	
	
	h = addCode(h,sentence,addr,NEXT);
	return h;	
}

struct CodeNode* genControlCode(struct CodeNode* h,int bool_e,int addr)
{
	char sentence[200]="";
	char* pre = "if  &";
	char*mid = " goto ";
	char num[5];
	sprintf(num,"%d",bool_e);
			
	strcat(sentence,pre);
	strcat(sentence,num);
	strcat(sentence,mid);
	h = addCode(h,sentence,addr,NEXT);	
	
	return h;
}

struct CodeNode* genGotoCode(struct CodeNode* h,int addr)
{
	char sentence[200]="";
	char*mid = "goto ";
	strcat(sentence,mid);
	h = addCode(h,sentence,addr,NEXT);	
	
	return h;
}

struct CodeNode* genFunCode(struct CodeNode* h,int addr,char* name)
{
	char sentence[200]="";
	char*mid = "FUNCTION &";
	strcat(sentence,mid);
	strcat(sentence,name);
	h = addCode(h,sentence,addr,NEXT);	
	
	return h;
}


struct CodeNode* genReturnCode(struct CodeNode* h,int addr,char* p_addr)
{
	char sentence[20] = "RETURN  ";
	strcat(sentence,p_addr);
	h = addCode(h,sentence,addr,NEXT);
	return h;	
}


void editCode(struct CodeNode* h,int goto_addr)
{
	
	struct CodeNode *temp = h;
	h->_goto = goto_addr;
}



struct CodeNode* addCode(struct CodeNode* h,char* sentence,int addr,int _goto)
{
	struct CodeNode* newCodeNode= NULL;
	struct CodeNode* temp = h;
	newCodeNode = (struct CodeNode*)malloc(sizeof(struct CodeNode));
	if(newCodeNode == NULL)
	{
		printf("fail in creating code node .\n");
		exit(0);
	}
	strcpy(newCodeNode->sentence,sentence);
	newCodeNode->addr = addr;
	newCodeNode->_goto = _goto;
	newCodeNode->next = NULL;
	
	if(h == NULL)
	h = newCodeNode;
	else
	{
		while(temp->next != NULL)
		temp = temp->next;
		temp->next = newCodeNode;		
	}
	return h;
	
}

int widen(int left,int right,int line)
{
	if(left == INT ||  left == CONST_INT)
	{
		if(right == INT || right == CONST_INT)
		{
			return right;
		}
		else if(right == FLOAT || right == CONST_FLOAT)
		{
			printf("Warning: Line:%d type \'int\' is converting to \'float\'\n",line);
			return right;
		}
		else
		{
			printf("Warning: Line:%d type \'char\' is converting to \'int\'\n",line);		
			return left;
		}
	}
	else if(left == FLOAT ||  left == CONST_FLOAT)
	{
		if(right == INT || right == CONST_INT)
		{
			printf("Warning: Line:%d type \'int\' is converting to \'float\'\n",line);		
		}
		else if(right == CHAR || right == CONST_CHAR)
		{
			printf("Warning: Line:%d type \'char\' is converting to \'float\'\n",line);		
		}		
		return left;
	}
	else if(left == CHAR ||  left == CONST_CHAR)
	{
		if(right == INT || right == CONST_INT)
		{
			printf("Warning: Line:%d type \'char\' is converting to \'int\'\n",line);		
		}
		else if(right == FLOAT || right == CONST_FLOAT)
		{
			printf("Warning: Line:%d type \'char\' is converting to \'float\'\n",line);		
		}		
		return right;
	}
	
	
}

void printCode(struct CodeNode* h)
{
	struct CodeNode* p = h;
	printf("------------------------Intermediate code -----------------------\n");
	
	while(p != NULL)
	{
		char* c = p->sentence;
		printf("%d :  ",p->addr);
		while(*c != '\0')
		printf("%c",*c++);
		if(p->_goto != NEXT)
		printf("%d ",p->_goto);
		printf("\n");
		p = p->next;
	}	
}

void writeCode(struct CodeNode* h)
{
	printf("\n----------------------------------\n");
	printf("Intemediate Code is generating. ....\n");
	
	 struct CodeNode* p = h;
	 FILE * oc = NULL;
	 if ( ( oc = fopen("objcode.s","w+") ) == NULL)
	 {
			printf("YuiSem->Error: Create file fail.\n");
			exit(0);
	 } 
	 
	while(p != NULL)
	{
		char* c = p->sentence;
		fprintf(oc,"%d :  ",p->addr);		
		while(*c != '\0')
		fprintf(oc,"%c",*c++);
		if(p->_goto != NEXT)
		fprintf(oc,"%d",p->_goto);
	    fprintf(oc,"\r\n");
		p = p->next;
	}
	printf("Done ^^\n");
}