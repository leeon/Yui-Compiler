#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "state.h"
#include "ylib.h"

/*the size of the token buffer*/
#define BUFFER_SIZE 200
/*the token buffer */
int buffer[BUFFER_SIZE];
 /*saving the line number*/
int buffer_line[BUFFER_SIZE]; 
/*the current pointer*/
int position = 0;
/*sementics type of a expression*/
int sem_type = 0;


/*to keep the current block number*/
int current_block = 0;
/*to name all the temp variable for the assemble language*/ 
int temp_order = 0;
/*to mark the address*/
int addr_order = 1000;
int loop_addr = 0;
/* mark the type of function*/
int type_toMatch = 0;
int current_type = 0;
/*for Test:use to stop program */
int stop= 0;

/*maintance the pointer*/
struct TreeNode* currentTree =  NULL;
int current_order = 1;
/***********************************Grammar Rules***********************************/

/*   
（1表达式文法   形如id+id*id）
              E->TE1
              E1->+TE1|~
              T->FT1
              T1->*FT1|~
			  F->MF1
			  F1-> =MF1|~
              M	->(E)|i
*/
 struct Link* E();
 struct Link* E1(struct Link* p_t);
 struct Link* T();
 struct Link* T1( struct Link* p_f);
 struct Link* F();
 struct Link*F1( struct Link* p_m);
 struct Link* M();

/* 
（2 IF ELSE文法 ）
			S→ if (E)  {MATCH}S1| E
			S1→~|else {MATCH}
	    	MATCH->  E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH  |~

*/
void S();
void S1();
void MATCH();

/* 
 (3赋值表达式文法)
			A ->  type id B1 B2 
			B1 -> =EB1 | ~
			B2 -> , id B1 B2 |~

*/
void A();
void B1(char* p_id);
void B2();


/* 
 (4函数声明描述文法)
		FUN -> type id (A)D1
		D1 -> { MATCH1}
		MATCH1 ->  E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH  | RT MATCH |~
		
*/
void FUN();
void D1();

/*
 （5 while循环文法描述）
		 W -> while(E)W1
		 W1 ->{ MATCH }
		 MATCH -> E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH  | RT MATCH  |~
*/
void W();
void W1();


/* 
  （6 for循环文法描述）
		FR -> for(E E E) FR1
		FR1 -> {MATCH}
		MATCH -> E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH  | RT MATCH |~
*/
void FR();
void FR1();

/*
 （7 参数列表文法描述）
		P ->type id P1
		P1 -> , id P1 |~
*/
void P();
void P1();


/*
 （8 return语句 文法描述）
		RT -> return E
*/
void RT();



/*语法分析执行函数*/
void parser()
{
     /*get all the token buffer*/
	  init_buffer();
	 /*init the block stack*/ 
	  initStack();
	  push(current_block);
	  /*Parsing begins!*/
      while(buffer[position] != END)
	  {
		  if(buffer[position] == IF)
		  {
			S();        /*IF ELSE BLOCK*/
			sem_type = IF_ELSE;
		  }
		  else if(buffer[position] == VOID)
		  {
			FUN();  /*Function BLOCK*/
			sem_type = FUN_BLOCK;
		  }
		  else if(buffer[position] == INT || buffer[position] == FLOAT || buffer[position] == CHAR)
		  {
					if(buffer[position+2] == SLP)
					{
						FUN();/*Function BLOCK*/
						sem_type = FUN_BLOCK;
					}
					else
					{
						A();/*Assignment BLOCK*/
						sem_type = ASSIGN;
					}
		  }
		  else if(buffer[position] == WHILE)
		  {
			W();  /*While BLOCK*/
			sem_type = WHILE_BLOCK;
		  }
		  else if(buffer[position] == FOR)
		  {
			FR();   /*For circle BLOCK*/
			sem_type = FOR_BLOCK;
		  }
		  else if(buffer[position] == RETURN)
		  {
			RT();  /*RETURN EXPRESSION*/
			sem_type = RETURN_EXPRESSION;
		  }
		  else
		  {
			E();  /*Regular expression*/
			sem_type = EXPRESSION;
		  }

		  /*get the result */
			if(Syntax_Error == NO)
			{
				printf("******************\nSyntax Success!\n******************\n");
			}
			else
			{
			    if(buffer[position] != END)
				position++;
				printf("******************\nSyntax Failure!\n******************\n");
			}
			if(Sem_Error == NO)
			{
				printf("******************\nSementics Success!\n******************\n");	
			}
			else
			{
				printf("******************\nSementics Failure!\n******************\n");					
			}
	
	  }	
	  
	 printf("YuiSyntax->Syntax analyzing finished! \n");
	 printf("YuiSementics->Sementics analyzing finished! \n");	 
}


void init_buffer()
{
		struct Node* token_buffer = Token;
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("YuiSyntax->Initializing the token buffer .....\n");
		while(token_buffer != NULL)
		{
			if(token_buffer->value != LC && token_buffer->value !=RC)
			{			
				//从缓冲区获取一个token
				buffer[position] =  token_buffer->value;
				buffer_line[position] =  token_buffer->line;
				position++;
			}
			token_buffer = token_buffer->next;
		}
		
		buffer[position] = END;
		position = 0;/*指针回到buffer头*/
		printf("YuiSyntax->Syntax analyzing begins .....\n");
		
		/*开始构建语法分析树*/
		SyntaxTree = initTree(SyntaxTree);
		currentTree= SyntaxTree;
}

/*********************************（1表达式文法   形如id+id*id）******************************************/
 struct Link* T1( struct Link* p_f)
   {
        struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 
		/*add tree node to the tree*/
		char name[10]="T1";
		currentTree = addTree(currentTree,current_order++,VN,name);
		/*add end*/
  		
		if(buffer[position] == ADD || buffer[position] == SUB)
		{
				/*add tree node to the tree*/
				char name[10];
				if(buffer[position] == ADD)
				name[0] = '+';
				else
				name[0] = '-';
				name[1]='\0';
				currentTree = addTree(currentTree,current_order++,buffer[position],name);
				/*add end*/
		
				printf("T1 -> +FT1\n");
				position++;
				 struct Link* f=F();

				
			   char temp_name[20]="temp";
			   char num[5];
			   sprintf(num,"%d",temp_order++);
			   strcpy(link->addr,temp_name);
			   strcat(link->addr,num);
			   char *OP1 = "=";	
			   link->type = widen(p_f->type,f->type,buffer_line[position]);
			   Code= emit(Code,addr_order++,link->addr,OP1,p_f->addr,name,f->addr);	

			   struct Link*t1=T1(link);
		}
		else
		{

			/*add tree node to the tree*/
			char name[10]="~";
			currentTree = addTree(currentTree,current_order++,BLANK,name);
			/*add end*/				
			printf("T1 -> ~\n");
		}
		/*back the pointer*/
		currentTree = currentTree->parent;
		return link;
   }

 struct Link* M()
   {
        struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 
		/*add tree node to the tree*/
		char name[10]="M";
		currentTree = addTree(currentTree,current_order++,VN,name);
		/*add end*/	
			
		if(buffer[position] == SLP)
		{
			/*add tree node to the tree*/
			char name[10]="(";
			currentTree = addTree(currentTree,current_order++,SLP,name);
			/*add end*/	
		
		    printf("M -> (E)\n");
			position++;
			struct Link*e=E();
			strcpy(link->addr,e->addr);
			strcpy(link->id,e->id);
			link->type = e->type;
		   if(buffer[position] == SRP)
			{
						/*add tree node to the tree*/
						char name[10]=")";
						currentTree = addTree(currentTree,current_order++,SRP,name);
						/*add end*/	
						position++;
			}
			else
		    {
					printf("Error: Line:%d  Expected \')\' \n",buffer_line[position-1]);
					Syntax_Error = YES;
			}
		}

		else if(buffer[position] == IDN || buffer[position] == CONST_INT || buffer[position] == CONST_FLOAT || buffer[position] == CONST_CHAR)
		{

					/*add tree node to the tree*/
					int i =0;
					char name[30];
					struct Node *temp = Token;
					/*查找符号表*/
				    while( temp->value ==LC || temp->value == RC|| i++ != position)
					temp = temp->next;			
					strcpy(name,temp->name);
					currentTree = addTree(currentTree,current_order++,buffer[position],name);
					/*add end*/	
					/*check identifier*/
					if(buffer[position] == IDN )
					{
							int isFind = NO;
							temp = Table;
							while(temp->value != IDN)
							temp = temp->next;
							while(temp != NULL)
							{
								if(strcmp(temp->name,name)==0)
								if(searchStack(temp->block_num) == YES)
								{								
									isFind = YES;
									link->type = temp->type;
								}
								temp = temp->next;	
							}
							if(isFind == NO)
							{
								printf("Error: Line:%d  Undefined variable found at %c^\n",buffer_line[position],*name);
								Sem_Error = YES;
							}
							else
							{
								strcpy(link->id,name);
								strcpy(link->addr,name);
							}
					}
					else
					{
								link->type = buffer[position];
								strcpy(link->id,name);
								strcpy(link->addr,name);
					}
					/*check end*/
					printf("M -> id\n");
					position++;
		}
		else
		{
					printf("Error: Line:%d  Expected \'(\' or id \n",buffer_line[position-1]);
					Syntax_Error = YES;
		}
		/*back the pointer*/
		currentTree = currentTree->parent;
		return link;
   }
   
    
 struct Link*  F()
   {
        struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 
		
     	/*add tree node to the tree*/
		char name[10]="F";
		currentTree = addTree(currentTree,current_order++,VN,name);
		/*add end*/	
        printf("F->MF1\n");
		 struct Link* m=M();
		 struct Link* f1=F1(m);

		/*code */
		char* test ="";
		if(strcmp(test,f1->addr) != 0)
		{
			strcpy(link->addr,f1->addr); /*F.link->addr :=  *M.link->addr*/
			link->type = f1->type;
		}
		else
		{
			strcpy(link->addr,m->addr); 
			link->type = m->type;
		}
		strcpy(link->id,m->id);		
		/*back the pointer*/
		currentTree = currentTree->parent;
		return link;
   }
   
 struct Link*F1( struct Link* p_m)
  {
        struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 
		/*add tree node to the tree*/
		char name[10]="F1";
		currentTree = addTree(currentTree,current_order++,VN,name);
		/*add end*/
		
		if(buffer[position] == MUL || buffer[position] == DIV)
		{
				/*add tree node to the tree*/
			char name[10];
			if(buffer[position] == MUL)
			name[0] = '*';
			else
			name[0] = '/';
			name[1]='\0';
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/
				
			printf("F1 -> *MF1\n");
			position++;
			 struct Link* m=M();

			
			char temp_name[20]="temp";
			char num[5];
			sprintf(num,"%d",temp_order++);
			strcpy(link->addr,temp_name);
			strcat(link->addr,num);
			char *OP = "=";  		
			link->type = widen(p_m->type,m->type,buffer_line[position]);
			Code= emit(Code,addr_order++,link->addr,OP,p_m->addr,name,m->addr);
		
		
			 struct Link* f1=F1(link);
		}
		else
		{
			/*add tree node to the tree*/
			char name[10]="~";
			currentTree = addTree(currentTree,current_order++,SLP,name);
			/*add end*/				
			printf("F1 -> ~\n");

		}
		currentTree = currentTree->parent;
		return link;
  }
	
	
 struct Link*T()
  {
        struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 

  		/*add tree node to the tree*/
		char name[10]="T";
		currentTree = addTree(currentTree,current_order++,VN,name);
		/*add end*/	
        printf("T -> FT1\n");
		 struct Link*f=F();
		 struct Link* t1=T1(f);
		
		/*code */
		char* test = "";
		if(strcmp(test,t1->addr) != 0)
		{
			strcpy(link->addr,t1->addr); /*F.link->addr :=  *M.link->addr*/
			link->type = t1->type;
		}
		else
		{
			strcpy(link->addr,f->addr);
			link->type = f->type;
		}
		strcpy(link->id,f->id); /*T.addr->addr :=  F.addr->addr*/
		/*code */	
			
		/*back the pointer*/
		currentTree = currentTree->parent;
		return link;
  }
  
 struct Link*E1(struct Link* p_t)
  {
        struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 
	/*add tree node to the tree*/
		char name[10]="E1";
		currentTree = addTree(currentTree,current_order++,VN,name);
		/*add end*/	
		
		if(buffer[position] == EQ || buffer[position] == NE|| buffer[position] == EE || buffer[position] == GT || buffer[position] == LT || buffer[position] == LE ||buffer[position] == GE )
		{
			/*add tree node to the tree*/
			char name[10];
			switch(buffer[position])
			{
				case EQ:
					name[0]='=';
					name[1]='\0';
					break;
			    case NE:
					name[0]='!';
					name[1]='=';
					name[2]='\0';
					break;
				case EE:
					name[0]='=';
					name[1]='=';
					name[2]='\0';
					break;
				case GT:
					name[0]='>';
					name[1]='\0';
					break;
				case LT:	
					name[0]='<';
					name[1]='\0';
					break;
				case LE:	
					name[0]='>';
					name[1]='=';
					name[2]='\0';
					break;
				case GE:	
					name[0]='<';
					name[1]='=';
					name[2]='\0';
					break;			
			
			}
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/			
			printf("E1-> =TE1\n");
			position++;
		     struct Link* t=T();
			
			/*code */			
			link->type = widen(p_t->type,t->type,buffer_line[position]);
			Code= gencode(Code,addr_order++,p_t->id,name,t->addr);
			
			 struct Link* e1=E1(link);
		}
		else
		{
			/*add tree node to the tree*/
			char name[10]="~";
			currentTree = addTree(currentTree,current_order++,BLANK,name);
			/*add end*/			
			printf("T1 -> ~\n");
		}
		/*back the pointer*/
		currentTree = currentTree->parent;  
		return link;
  }
  
 struct Link* E()
  {
        struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 
		printf("--------------------\nExpreesions ! Recognised!\n--------------------\n");
        printf("E -> TE1\n");
		/*add tree node to the tree*/
		char name[10]="E";
		currentTree = addTree(currentTree,current_order++,VN,name);
		/*add end*/
		 struct Link* t=T();
		 struct Link* e1=E1(t);
		/*code */
		strcpy(link->addr,t->addr); /*E.link->addr = T.link->addr*/
		strcpy(link->id,t->id);
		link->type = t->type;
		//Code = gencode(Code,addr_order++,link->addr,e1);/*gencode(E.link->addr,E1.link->addr)*/
		/*code */
		if(buffer[position] == SEMI)
		{
			char name[10]=";";
			currentTree = addTree(currentTree,current_order++,SEMI,name);
			position++;

		}
		/*back the pointer*/
		currentTree = currentTree->parent;
		return link;
  }
  
 /************************************（2 IF ELSE文法 ）*******************************************/ 
  
void S()
{

	/*add tree node to the tree*/
	char name[10]="S";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/		
	
	
	if(buffer[position] == IF)
	{
		/*add tree node to the tree*/
		char name[10]="if";
		currentTree = addTree(currentTree,current_order++,IF,name);
		/*add end*/	
	
		printf("--------------------\nIf_else Block !Recognised!\n--------------------\n");
	//scanf("%d",&stop);
		printf("S -> if (E){S}S1\n");
		position++;
        
		if(buffer[position] == SLP)
		{
			/*add tree node to the tree*/
			char name[10]="(";
			currentTree = addTree(currentTree,current_order++,SLP,name);
			/*add end*/	
			position++;
			/*begin if */
			struct Link* e=E();
			Code = genControlCode(Code,addr_order-2,addr_order++);	
			Code = genGotoCode(Code,addr_order++);
			
		    struct CodeNode* temp =Code;
			while(temp->next->next != NULL)
			temp = temp->next;
			struct CodeNode *truelist = temp;
			struct CodeNode *falselist = temp->next;
			editCode(truelist,addr_order);
			
			
			if(buffer[position] == SRP)
			{
				/*add tree node to the tree*/
				char name[10]=")";
				currentTree = addTree(currentTree,current_order++,SRP,name);
				/*add end*/	
				position++;
				if(buffer[position] == LP)
				{
					/*control the block number*/
					current_block++;
					push(current_block);
					
					/*add tree node to the tree*/
					char name[10]="{";
					currentTree = addTree(currentTree,current_order++,LP,name);
					/*add end*/					
					position++;
					
					MATCH();
					Code = genGotoCode(Code,addr_order++);
					while(temp->next!= NULL)
					temp = temp->next;
					struct CodeNode *endlist = temp;

				
					//scanf("%d",&stop);
					if(buffer[position] == RP)
					{
						editCode(falselist,addr_order);
						/*control the block number*/
						pop();
						/*add tree node to the tree*/
						char name[10]="}";
						currentTree = addTree(currentTree,current_order++,RP,name);
						/*add end*/	
						position++;
						S1();
						editCode(endlist,addr_order);
					}
					else
					{
					printf("Error: Line:%d  Expected \'}\' \n",buffer_line[position-1]+1);
					Syntax_Error = YES;		
					}
				}
				else
				{
				//scanf("%d",&stop);
					printf("Error: Line:%d  Expected \'{\' \n",buffer_line[position]);
					Syntax_Error = YES;					
				}
			}
			else
			{
			//scanf("%d",&stop);
				printf("Error: Line:%d  Expected \')\' \n",buffer_line[position-1]);
			    Syntax_Error = YES;
			}
		}
		else
		{
		//scanf("%d",&stop);
			printf("Error: Line:%d  Expected \'(\' \n",buffer_line[position]);
			Syntax_Error = YES;
		}
	}
	else
	{
		/*add tree node to the tree*/
		char name[10]="~";
		currentTree = addTree(currentTree,current_order++,BLANK,name);
		/*add end*/	
	    printf("S1  ->  E\n");
		E();
	}
	/*back the pointer*/
	currentTree = currentTree->parent;
}
void S1()
{
	/*add tree node to the tree*/
	char name[10]="S1";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/	
	if(buffer[position] == ELSE)
	{
		/*add tree node to the tree*/
		char name[10]="else";
		currentTree = addTree(currentTree,current_order++,ELSE,name);
		/*add end*/		
	    printf("S1  ->  else{S}\n");
		position++;
		
		if(buffer[position] == LP)
		{
			/*control the block number*/
			current_block++;
			push(current_block);		
			/*add tree node to the tree*/
			char name[10]="{";
			currentTree = addTree(currentTree,current_order++,LP,name);
			/*add end*/			
			position++;
			MATCH();
			if(buffer[position] == RP)
			{
			pop();
		/*add tree node to the tree*/
				char name[10]="}";
				currentTree = addTree(currentTree,current_order++,RP,name);
				/*add end*/				
				position++;
			}
			else
			{
			  printf("Error: Line:%d  Expected \'}\' \n",buffer_line[position-1]+1);
			  Syntax_Error = YES;					
			}
		}
		else
		{
			printf("Error: Line:%d  Expected \'{\' \n",buffer_line[position]);
			Syntax_Error = YES;			
		}
	}
	else
	{
		/*back the pointer*/
		currentTree = currentTree->parent;
		printf("S1 -> ~\n");
	}
}



/**********************************(3赋值表达式文法)***********************************************/

void A()
{
  struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 
	/*add tree node to the tree*/
	char name[10]="A";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/	
	if(buffer[position] == INT || buffer[position] == FLOAT || buffer[position] == CHAR)
	{
				current_type = buffer[position];
				char name[10];
				switch(buffer[position])
				{
					case INT:
						name[0]='i';
						name[1]='n';
						name[2]='t';
						name[3]='\0';
						break;
					case FLOAT:
						name[0]='f';
						name[1]='l';
						name[2]='o';
						name[3]='a';
						name[4]='t';
						name[5]='\0';
						break;
					case CHAR:
						name[0]='c';
						name[1]='h';
						name[2]='a';
						name[3]='r';
						name[4]='\0';
						break;					
				}
		/*add tree node to the tree*/
		currentTree = addTree(currentTree,current_order++,buffer[position],name);
		/*add end*/
		printf("--------------------\nVariable Assignment! !Recognised!\n--------------------\n");
		printf("A ->  type id B1 B2 \n");
		position++;
		
		if(buffer[position] == IDN)
		{
			/*add tree node to the tree*/
			int i =0;
			char name[30];
			struct Node *temp = Token;
			/*查找符号表*/
		    while( temp->value ==LC || temp->value == RC|| i++ != position)
			temp = temp->next;			
			strcpy(name,temp->name);
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/
			
			
			/*check identifier*/
					int isFind = NO;
					temp = Table;
					while(temp->value != IDN)
					temp = temp->next;
					while(temp != NULL)
					{
						if(strcmp(temp->name,name)==0)
						if(temp->block_num == current_block)
						{
							isFind = YES;
						}
						temp = temp->next;	
					}
					if(isFind == YES)
					{
						printf("Error: Line:%d  Redefined variable found at %c^\n",buffer_line[position],*name);
						Sem_Error = YES;
					}					
					else
					{
					      strcpy(link->id,name);
				          /*add the assignment node*/				
						Table = add(Table,name,buffer[position],buffer_line[position],current_block,current_type);					
					}
                   /*check end*/			

			position++;
			B1(link->id);
			B2();
			if(buffer[position] == SEMI)
			{
				char name[10]=";";
				currentTree = addTree(currentTree,current_order++,SEMI,name);			
				position++;
			}
		}
		else
		{
			printf("Error: Line:%d  Missing an identifier !",buffer_line[position]);
			Syntax_Error = YES;		
		}
	}
	else
	{
		printf("Error: Line:%d  Reference to a type !",buffer_line[position]);
		Syntax_Error = YES;		
	}
	/*back the pointer*/
	currentTree = currentTree->parent;
}


void B1(char* p_id)
{
    struct Link* link= (struct Link* )malloc(sizeof(struct Link)); 
	/*add tree node to the tree*/
	char name[10]="B1";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/	
	if(buffer[position] == EQ)
	{
		/*add tree node to the tree*/
		char name[10]="=";
		currentTree = addTree(currentTree,current_order++,EQ,name);
		/*add end*/		
		printf("B1 -> =EB1 \n");
		position++;
		 struct Link* e=E();
         char* OP ="=";
		 Code= gencode(Code,addr_order++,p_id,OP,e->addr);
		B1(p_id);
	}
	else
	{
		/*add tree node to the tree*/
		char name[10]="~";
		currentTree = addTree(currentTree,current_order++,BLANK,name);
		/*add end*/		
		printf("B1 -> ~\n");
	}
	/*back the pointer*/
	currentTree = currentTree->parent;
}
  
  
void B2()
{
	/*add tree node to the tree*/
	char name[10]="B2";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/	
	if(buffer[position] == COM)
	{
		/*add tree node to the tree*/
		char name[10]=",";
		currentTree = addTree(currentTree,current_order++,COM,name);
		/*add end*/	
	
		printf("B2 -> , id B1 B2\n");
		position++;
		if(buffer[position] == IDN)
		{
			/*add tree node to the tree*/
			int i =0;
			char name[30];
			struct Node *temp = Token;
			/*查找符号表*/
		    while( temp->value ==LC || temp->value == RC|| i++ != position)
			temp = temp->next;			
			strcpy(name,temp->name);
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/
			
			/*check identifier*/
					int isFind = NO;
					temp = Table;
					while(temp->value != IDN)
					temp = temp->next;
					while(temp != NULL)
					{
						if(strcmp(temp->name,name)==0)
						if(temp->block_num == current_block)
						{
							isFind = YES;
						}
						temp = temp->next;	
					}
					if(isFind == YES)
					{
						printf("Error: Line:%d  Redefined variable found at %c^\n",buffer_line[position],*name);
						Sem_Error = YES;
					}					
					else
					{
				          /*add the assignment node*/				
						Table = add(Table,name,buffer[position],buffer_line[position],current_block,current_type);					
					}
                   /*check end*/	
			
			position++;
			char *space =NULL;
			B1(space);
			B2();
		}
		else
		{
		scanf("%d",&stop);
			printf("Error: Line:%d  Missing an identifier !",buffer_line[position]);
			Syntax_Error = YES;		
		}
	}
	else
	{
		/*add tree node to the tree*/
		char name[10]="~";
		currentTree = addTree(currentTree,current_order++,BLANK,name);
		/*add end*/	
		printf("B2 -> ~\n");
	}
	/*back the pointer*/
	currentTree = currentTree->parent;	
}

/*************************************(4函数声明描述文法)********************************************/
void FUN()
{
	/*add tree node to the tree*/
	char name[10]="FUN";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/		
	if(buffer[position] == INT || buffer[position] == FLOAT || buffer[position] == CHAR || buffer[position]==VOID)
	{
	            type_toMatch = buffer[position];
				
				char name[10];
				switch(buffer[position])
				{
					case INT:
						name[0]='i';
						name[1]='n';
						name[2]='t';
						name[3]='\0';
						break;
					case FLOAT:
						name[0]='f';
						name[1]='l';
						name[2]='o';
						name[3]='a';
						name[4]='t';
						name[5]='\0';
						break;
					case CHAR:
						name[0]='c';
						name[1]='h';
						name[2]='a';
						name[3]='r';
						name[4]='\0';
						break;			
					case VOID:
						name[0]='v';
						name[1]='o';
						name[2]='i';
						name[3]='d';
						name[4]='\0';
						break;									
				}
		/*add tree node to the tree*/
		currentTree = addTree(currentTree,current_order++,buffer[position],name);
		/*add end*/

		
		printf("--------------------\nFunction Assignment! !Recognised!\n--------------------\n");
		printf("FUN -> type id (P)D1 \n");
		position++;
	//	printf("%d\n",buffer[position]);
		if(buffer[position] == IDN)
		{
			/*add tree node to the tree*/
			int i =0;
			char name[30];
			struct Node *temp = Token;
			/*查找符号表*/
		    while( temp->value ==LC || temp->value == RC|| i++ != position)
			temp = temp->next;			
			strcpy(name,temp->name);
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			
			/*add end*/	
			Code = genFunCode(Code,addr_order++,name);
			position++;
			if(buffer[position] == SLP)
			{
				/*control the block number*/
				current_block++;
				push(current_block);				
				/*add tree node to the tree*/
				char name[10]="(";
				currentTree = addTree(currentTree,current_order++,SLP,name);
				/*add end*/
				
				position++;
				P();
				if(buffer[position] == SRP)
				{
				    pop();
					/*add tree node to the tree*/
					char name[10]=")";
					currentTree = addTree(currentTree,current_order++,SRP,name);
					/*add end*/
				
					position++;
					D1();
				}
				else
				{
					printf("Error: Line:%d  Expected \')\' \n",buffer_line[position-1]);
					Syntax_Error = YES;		
				}
			}
			else
			{
				printf("Error: Line:%d  Expected \'(\' \n",buffer_line[position]);
				Syntax_Error = YES;				
			}
		}
		else
		{
		
			printf("Error: Line:%d  Missing an identifier !",buffer_line[position]);
			Syntax_Error = YES;					
		}
		
	}
	else
	{
		printf("Error: Line:%d  Reference to a type !",buffer_line[position]);
		Syntax_Error = YES;		
	}
	/*back the pointer*/
	currentTree = currentTree->parent;		
}

void D1()
{
	/*add tree node to the tree*/
	char name[10]="D1";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/		
	
	if(buffer[position] == LP)
	{
		/*control the block number*/
		push(current_block);
	/*add tree node to the tree*/
		char name[10]="{";
		currentTree = addTree(currentTree,current_order++,LP,name);
		/*add end*/			
		printf("D1 ->{ MATCH }\n");
		position++;
		MATCH();
		if(buffer[position] == RP)
		{
			pop();
			/*add tree node to the tree*/
			char name[10]="}";
			/*out of a function*/
			type_toMatch = 0;
			currentTree = addTree(currentTree,current_order++,RP,name);
			/*add end*/				
			position++;
		}
		else
		{
			printf("Error: Line:%d  Expected \'}\' or id \n",buffer_line[position-1]+1);
			Syntax_Error = YES;					
		}
	}
	else
	{
		printf("Error: Line:%d  Expected \'{\' or id \n",buffer_line[position]);
		Syntax_Error = YES;		
	}
	/*back the pointer*/
	currentTree = currentTree->parent;	
	
}

void MATCH()
{
	/*add tree node to the tree*/
	char name[10]="MATCH";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/		
	
	if(buffer[position] == IF)
		  {				  	  
		    printf("MATCH -> S MATCH \n");
			S();        /*IF ELSE BLOCK*/
			MATCH();
			sem_type = IF_ELSE;
		  }
		  else if(buffer[position] == INT || buffer[position] == FLOAT || buffer[position] == CHAR)
		  {
		  		if(buffer[position+2] != SLP)
				{		
					printf("MATCH -> A MATCH \n");
					A();
					MATCH();
					sem_type = ASSIGN;
					//变量定义声明
				}
			}
		  else if(buffer[position] == WHILE)
		  {			  
			printf("MATCH -> W MATCH \n");
			W();
			MATCH();
			sem_type = WHILE_BLOCK;
		  }
		  else if(buffer[position] == IDN || buffer[position] == CONST_INT || buffer[position] == CONST_FLOAT || buffer[position] == CONST_CHAR)
		  {		
		   printf("MATCH -> E MATCH \n");
			E();  /*表达式文法模式*/
			MATCH();
			sem_type = EXPRESSION;
		  }
		  
		  else if(buffer[position] == FOR)
		  {		  
			FR();
			MATCH();
			sem_type = FOR_BLOCK;
		  }
		  
		  else if(buffer[position] == RETURN)
		  {

			RT();
			sem_type = RETURN_EXPRESSION;
		  }
		  else
		  {
			/*add tree node to the tree*/
			char name[10]="~";
			currentTree = addTree(currentTree,current_order++,BLANK,name);
			/*add end*/		
			printf("MATCH -> ~\n");
		  }
		  
	 
	/*back the pointer*/
	currentTree = currentTree->parent;	
	
}
/***********************************（5 while循环文法描述）**********************************************/

void W()
{
	/*add tree node to the tree*/
	char name[10]="W";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/	
	if(buffer[position] == WHILE)
	{
		/*add tree node to the tree*/
		char name[10]="while";
		currentTree = addTree(currentTree,current_order++,WHILE,name);
		/*add end*/		
		printf("--------------------\nWhile Block! !Recognised!\n--------------------\n");
		printf("W -> while(E) W1\n");
		position++;
		if(buffer[position] == SLP)
		{	
			/*add tree node to the tree*/
			char name[10]="(";
			currentTree = addTree(currentTree,current_order++,SLP,name);
			/*add end*/	
			
			position++;
			loop_addr = addr_order;
			E();
			
			Code = genControlCode(Code,addr_order-2,addr_order++);	
			Code = genGotoCode(Code,addr_order++);
			
		    struct CodeNode* temp =Code;
			while(temp->next->next != NULL)
			temp = temp->next;
			struct CodeNode *truelist = temp;
			struct CodeNode *falselist = temp->next;
			editCode(truelist,addr_order);
			
			
			if(buffer[position] == SRP)
			{

				/*add tree node to the tree*/
				char name[10]=")";
				currentTree = addTree(currentTree,current_order++,SRP,name);
				/*add end*/				
				position++;
				W1();
			    editCode(falselist,addr_order);				
			}
			else
			{
				printf("Error: Line:%d  Expected \')\' \n",buffer_line[position-1]);
				Syntax_Error = YES;				
			}
		}
		else
		{
			printf("Error: Line:%d  Expected \'(\' or id \n",buffer_line[position]);
			Syntax_Error = YES;
		}
	}
	else
	{
		printf("Error: Line:%d  We cannot find while or func name !",buffer_line[position]);
		Syntax_Error = YES;		
	}
	/*back the pointer*/
	currentTree = currentTree->parent;	
}

void W1()
{
	/*add tree node to the tree*/
	char name[10]="W1";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/	
	if(buffer[position] == LP)
	{
		/*control the block number*/
		current_block++;
		push(current_block);	
		/*add tree node to the tree*/
		char name[10]="{";
		currentTree = addTree(currentTree,current_order++,LP,name);
		/*add end*/		
		printf("W1 ->{ MATCH }\n");
		position++;
		MATCH();
		if(buffer[position] == RP)
		{
		
			Code = genGotoCode(Code,addr_order++);
		    struct CodeNode* temp =Code;
			while(temp->next!= NULL)
			temp = temp->next;
			struct CodeNode *looplist = temp;
			editCode(looplist,loop_addr);
			pop();
			/*add tree node to the tree*/
			char name[10]="}";
			currentTree = addTree(currentTree,current_order++,RP,name);
			/*add end*/			
			position++;
		}
		else
		{		
			printf("Error: Line:%d  Expected \'}\' or id \n",buffer_line[position-1]+1); /*token尚未保存此行，回退*/
			Syntax_Error = YES;					
		}
	}
	else
	{
		printf("Error: Line:%d  Expected \'{\' or id \n",buffer_line[position-1]);
		Syntax_Error = YES;		
	}
	/*back the pointer*/
	currentTree = currentTree->parent;		
}


/**********************************（6 for循环文法描述）***********************************************/
void FR()
{
	/*add tree node to the tree*/
	char name[10]="FR";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/		
	if(buffer[position] == FOR)
	{
		/*add tree node to the tree*/
		char name[10]="for";
		currentTree = addTree(currentTree,current_order++,FOR,name);
		/*add end*/		
		
		printf("--------------------\nFor Block! !Recognised!\n--------------------\n");
		printf("FR -> for(E E E) FR1\n");
		position++;
		if(buffer[position] == SLP)
		{
		/*add tree node to the tree*/
		char name[10]="(";
		currentTree = addTree(currentTree,current_order++,SLP,name);
		/*add end*/				
			position++;
			E();
			loop_addr = addr_order;
			E();
			
			Code = genControlCode(Code,addr_order-2,addr_order++);	
			Code = genGotoCode(Code,addr_order++);
			
		    struct CodeNode* temp =Code;
			while(temp->next->next != NULL)
			temp = temp->next;
			struct CodeNode *truelist = temp;
			struct CodeNode *falselist = temp->next;
			
			int step = addr_order;
			E();
			Code = genGotoCode(Code,addr_order++);
			while(temp->next!= NULL)
			temp = temp->next;
			struct CodeNode *looplist = temp;
			editCode(looplist,loop_addr);
			
			editCode(truelist,addr_order);
			if(buffer[position] == SRP)
			{
			
				/*add tree node to the tree*/
				char name[10]=")";
				currentTree = addTree(currentTree,current_order++,SRP,name);
				/*add end*/					
				position++;
				FR1();
				Code = genGotoCode(Code,addr_order++);
				while(temp->next!= NULL)
				temp = temp->next;
				struct CodeNode *endlist = temp;
				editCode(endlist,step);
				editCode(falselist,addr_order);
			}
			else
			{
				printf("Error: Line:%d  Expected \')\' !\n",buffer_line[position]);
				Syntax_Error = YES;	
			}
		}
		else
		{
			printf("Error: Line:%d  Expected \'(\' !\n",buffer_line[position]);
			Syntax_Error = YES;				
		}
	}
	else
	{
		printf("Error: Line:%d  We cannot find for or func name !\n",buffer_line[position]);
		Syntax_Error = YES;	
	}
	/*back the pointer*/
	currentTree = currentTree->parent;	
	
}

void FR1()
{
	/*add tree node to the tree*/
	char name[10]="FR1";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/	
	if(buffer[position] == LP)
	{
	/*control the block number*/
	current_block++;
	push(current_block);	
	/*add tree node to the tree*/
	char name[10]="{";
	currentTree = addTree(currentTree,current_order++,LP,name);
	/*add end*/	
	
		printf("FR1 -> {MATCH}\n");
		position++;
		MATCH();
		if(buffer[position] == RP)
		{
			pop();
			/*add tree node to the tree*/
			char name[10]="}";
			currentTree = addTree(currentTree,current_order++,RP,name);
			/*add end*/			
			position++;
		}
		else
		{		
			printf("Error: Line:%d  Expected \'}\' or id \n",buffer_line[position-1]+1);
			Syntax_Error = YES;					
		}
	}
	else
	{
		printf("Error: Line:%d  Expected \'{\' or id \n",buffer_line[position-1]);
		Syntax_Error = YES;		
	}
		/*back the pointer*/
	currentTree = currentTree->parent;	
}



/***********************************（7 参数列表文法描述）**********************************************/

void P()
{
	/*add tree node to the tree*/
	char name[10]="P";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/		
	printf("--------------------\nParameter List Block! !Recognised!\n--------------------\n");
	if(buffer[position] == SRP)
	{	
		printf("Null Parameters\n");
	}
	else if(buffer[position] == INT || buffer[position] == FLOAT || buffer[position] == CHAR)
	{
	                current_type = buffer[position];
					char name[10];
					switch(buffer[position])
					{
						case INT:
							name[0]='i';
							name[1]='n';
							name[2]='t';
							name[3]='\0';
							break;
						case FLOAT:
							name[0]='f';
							name[1]='l';
							name[2]='o';
							name[3]='a';
							name[4]='t';
							name[5]='\0';
							break;
						case CHAR:
							name[0]='c';
							name[1]='h';
							name[2]='a';
							name[3]='r';
							name[4]='\0';
							break;									
					}
			/*add tree node to the tree*/
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/		
		printf("P ->type id P1\n");
		position++;
		if(buffer[position] == IDN)
		{
			/*add tree node to the tree*/
			int i =0;
			char name[30];
			struct Node *temp = Token;
			/*查找符号表*/
		    while( temp->value ==LC || temp->value == RC|| i++ != position)
			temp = temp->next;			
			strcpy(name,temp->name);
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/

			/*check identifier*/
					int isFind = NO;
					temp = Table;
					while(temp->value != IDN)
					temp = temp->next;
					while(temp != NULL)
					{
						if(strcmp(temp->name,name)==0)
						if(temp->block_num == current_block)
						{
							isFind = YES;
						}
						temp = temp->next;	
					}
					if(isFind == YES)
					{
						printf("Error: Line:%d  Parameter name conflicts found at %c^\n",buffer_line[position],*name);
						Sem_Error = YES;
					}					
					else
					{
				          /*add the assignment node*/				
						Table = add(Table,name,buffer[position],buffer_line[position],current_block,current_type);					
					}
                   /*check end*/	
				   
			position++;
			P1();
		}
		else
		{
			printf("Error: Line:%d  Parameters Error! \n",buffer_line[position]);
			Syntax_Error = YES;						
		}
	}
	else
	{
		printf("Error: Line:%d  Parameters Error!  No Type!\n",buffer_line[position]);
		Syntax_Error = YES;		
	}
  	/*back the pointer*/
	currentTree = currentTree->parent;	
	
}

void P1()
{
	/*add tree node to the tree*/
	char name[10]="P1";
	currentTree = addTree(currentTree,current_order++,VN,name);
	/*add end*/
	
	if(buffer[position] == COM)
	{
		/*add tree node to the tree*/
		char name[10]=",";
		currentTree = addTree(currentTree,current_order++,COM,name);
		/*add end*/
		printf("P1 -> , id P1\n");
		position++;								
		
		if(buffer[position] == INT || buffer[position] == FLOAT || buffer[position] == CHAR)
		{
					current_type = buffer[position];
					char name[10];
					switch(buffer[position])
					{
						case INT:
							name[0]='i';
							name[1]='n';
							name[2]='t';
							name[3]='\0';
							break;
						case FLOAT:
							name[0]='f';
							name[1]='l';
							name[2]='o';
							name[3]='a';
							name[4]='t';
							name[5]='\0';
							break;
						case CHAR:
							name[0]='c';
							name[1]='h';
							name[2]='a';
							name[3]='r';
							name[4]='\0';
							break;									
					}
			/*add tree node to the tree*/
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/		
			
			printf("P ->type id P1\n");
			position++;
			if(buffer[position] == IDN)
			{
			/*add tree node to the tree*/
			int i =0;
			char name[30];
			struct Node *temp = Token;
			/*查找符号表*/
		    while( temp->value ==LC || temp->value == RC|| i++ != position)
			temp = temp->next;			
			strcpy(name,temp->name);
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/
			/*check identifier*/
					int isFind = NO;
					temp = Table;
					while(temp->value != IDN)
					temp = temp->next;
					while(temp != NULL)
					{
						if(strcmp(temp->name,name)==0)
						if(temp->block_num == current_block)
						{
							isFind = YES;
						}
						temp = temp->next;	
					}
					if(isFind == YES)
					{
						printf("Error: Line:%d  Parameter name conflicts found at %c^\n",buffer_line[position],*name);
						Sem_Error = YES;
					}					
					else
					{
				          /*add the assignment node*/				
						Table = add(Table,name,buffer[position],buffer_line[position],current_block,current_type);					
					}
                   /*check end*/				
				position++;
				P1();
			}
			else
			{
				printf("Error: Line:%d  Parameters Error! \n",buffer_line[position]);
				Syntax_Error = YES;						
			}
		}
		else
		{
			printf("Error: Line:%d  Parameters Error No Type! \n",buffer_line[position]);
			Syntax_Error = YES;					
		}
	}
	else
	{
		/*add tree node to the tree*/
		char name[10]="~";
		currentTree = addTree(currentTree,current_order++,BLANK,name);
		/*add end*/
		
		printf("P1 -> ~\n");
	}
  	/*back the pointer*/
	currentTree = currentTree->parent;	
}

/************************************（8 return语句文法描述）*********************************************/
void RT()
{
		/*add tree node to the tree*/
		char name[10]="RT";
		currentTree = addTree(currentTree,current_order++,VN,name);
		/*add end*/
		
	printf("--------------------\nReturn Block ! !Recognised!\n--------------------\n");
	if(buffer[position] == RETURN)
	{
		if(type_toMatch == 0)
		{
			printf("Error: Line:%d  illegal return expressions! \n",buffer_line[position]);
			Sem_Error = YES;			
		}
		/*add tree node to the tree*/
		char name[10]="return";
		currentTree = addTree(currentTree,current_order++,RETURN,name);
		/*add end*/
		
		printf("RT -> return E\n");
		position++;
		if(buffer[position] == IDN || buffer[position] == CONST_INT || buffer[position] == CONST_FLOAT || buffer[position] == CONST_CHAR)
		{
			/*add tree node to the tree*/
			int i =0;
			char name[30];
			struct Node *temp = Token;
			/*查找符号表*/
		    while( temp->value ==LC || temp->value == RC|| i++ != position)
			temp = temp->next;			
			strcpy(name,temp->name);
			currentTree = addTree(currentTree,current_order++,buffer[position],name);
			/*add end*/
			int temp_type =0;
			
				temp = Table;
				/*to speed up the program*/
				while(temp->value != IDN)
				temp = temp->next;
				while(temp != NULL)
				{
				
					if(strcmp(temp->name,name)==0)
					if(searchStack(temp->block_num) == YES)
					{
						temp_type = temp->type;
					}
					temp = temp->next;	
				}

				if(buffer[position]== IDN&&temp_type != type_toMatch && type_toMatch != 0)
				{
					printf("Error: Line:%d  return type doesn't match the function ! \n",buffer_line[position]);
					Sem_Error = YES;
				}
				if(buffer[position]!= IDN && (type_toMatch-buffer[position])!= 39 && type_toMatch != 0 )
				{

					printf("Error: Line:%d  return type doesn't match the function ! \n",buffer_line[position]);
                 	Sem_Error = YES;
				 }             
			
			
			struct Link* e=E();
			/*code*/
			Code = genReturnCode(Code,addr_order++,e->addr);
		}
		else
		{
			printf("Error: Line:%d  Missing Expressions! \n",buffer_line[position]);
			Syntax_Error = YES;					
		}
	}
	else
	{
			printf("Error: Line:%d  Missing Return! \n",buffer_line[position]);
			Syntax_Error = YES;				
	}
	/*back the pointer*/
	currentTree = currentTree->parent;	
}