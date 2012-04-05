#include<stdio.h>
#include<stdlib.h>
#include "state.h"
#include "ylib.h"


/*
Scanner.c
@author octobershiner
@version 0.1
2011 12 02
Description:  This file implements the basic scanning function
*/
#define BUFFER_SIZE 30            /*The Max length of the buffer*/
#define MAX_LINE 250              /*Define the capacity of each line*/ 
#define TOKEN_SIZE 30            /*Define the max token size*/


/*Define Keyword*/
char keywords[30][30] = {"int","float","char","return","while","for","if","else","void"};
												 
/*Operator*/
char symbol[40][30]= {"","","","","",
		                                          "{","}","+","-","/","*","=",
												  ">","<",";","(",")",
												  "!=","[","]","<=",">=","/*","*/","&&","||","==",",","&","|"}; 

/*用于保存验证匹配类型  默认为双引号*/
char toMatch = 34; 

/*用于标记 左右匹配情况 依次为 ( ) [] {} 注释符*/
int Lmatch[4] ={0,0,0,0};

/*用于记录是否开始注释*/
int Comment_Start = NO;

/*
*扫描函数
*/
												 
void scanner(char * file_name)
{
	 char current_line[MAX_LINE];       /*存储每一行的字符串*/ 
	 char * current_p = NULL;               /*当前字符指针*/
	 int line_number = 1;                                                                                                                                                                                                     
	 
     char token_str[TOKEN_SIZE];    /*用于存储每一个token串*/
	 int token_ptr = 0;  /*指向当前token串尾*/
	 
	 int last_state = START;  /*用于标记上一个token串的类型*/
	 int state = START;
	 
	 int add = YES;

	  
     /*开始 读取源代码文件*/
     FILE * fp = NULL;
	 if ( ( fp = fopen(file_name,"r") ) == NULL)  
	 {
			printf("YuiToken->Error: There is no such file.\n");
			Token_Error = YES;
			exit(0);
	 } 
	printf("YuiToken->Success: Source File loaded successfully!\n");
	
	
	/*扫描代码中的每一行*/
	 while(fgets(current_line,MAX_LINE,fp) != 0)
	 {
			printf("\nLine %d : \n",line_number);
			 current_p = current_line;                   /*重置当前指针*/
			 while(1)
			 {
			  
			     add = YES;
				 switch(state)
				 {
						case START:
						         /*处理空白字符*/
							     if(Comment_Start == YES && *(current_p) != '*' && *(current_p+1) != '/')
								 {
									 add =NO;
								 }
								 
						         else if(isBlank(*current_p) == YES)
								 {
									add = NO;
								 }
								 else if( isNumber(*current_p) == YES)
								{
									state = CONST_INT;
								}
								else if (*current_p ==39)/*遇到了单引号*/
								{
									state = CONST_CHAR;
								}
								else if(*current_p == 34)  /*遇到了双引号*/
								{
									state = CONST_STRING;
								}
								else if( isLetter(*current_p) == YES)
								{
									state = IDN;
								}
								else if(*current_p == '+' || *current_p =='-' ||*current_p == ';'||*current_p ==',')
								{
									last_state = isSingleOP(current_p);
									state = END;
								}
								else if(*current_p == '>' || *current_p == '<' || *current_p == '=')
								{
								    if( *(current_p+1) == '=')
									{
										if(*current_p == '>')
											state = GE;
										else if(*current_p == '<')
										    state = LE;
										else if(*current_p == '=')
										    state = EE;
									}
									else
										{	   
											last_state = isSingleOP(current_p);
											state = END;
										}
								}		
								else if(*current_p == '&')
                                {
									if( *(current_p+1) == '&')
									state = AND;
								    else
								    {
										last_state = isSingleOP(current_p);
										state = END;
									}
								}								
								else if(*current_p == '|')
                                {
									if( *(current_p+1) == '|')
									state = OR;
								    else
								    {									    
										last_state = isSingleOP(current_p);
										state = END;
									}
								}					
								else if(*current_p == '!')
                                {
									if( *(current_p+1) == '=')
									state = NE;
								    else
								    {							
										last_state = ERROR;
										state = END;
									}
								}
								else if(*current_p =='(')
								{
									Lmatch[0]++;
									last_state = SLP;
									state = END;
								}
 								else if(*current_p ==')')
								{
									if(Lmatch[0] > 0)  /*判断前面是否有左括号可匹配*/
									{
									    Lmatch[0]--;
										last_state = SRP;
										state = END;
									}
									else
									{
										toMatch = '(';
										last_state = UNMATCH;
										state = END;
									}	
								}
								else if(*current_p =='[')
								{
									Lmatch[1]++;
									last_state = ML;
									state = END;
								}
 								else if(*current_p ==']')
								{
									if(Lmatch[1] > 0)  /*判断前面是否有左中括号可匹配*/
									{
									    Lmatch[1]--;
										last_state = MR;
										state = END;
									}
									else
									{
										toMatch = '[';
										last_state = UNMATCH;
										state = END;
									}	
								}								
								else if(*current_p == '{')
								{
									Lmatch[2]++;
									last_state = LP;
									state = END;
								}
 								else if(*current_p =='}')
								{
									if(Lmatch[2] > 0)  /*判断前面是否有左大括号可匹配*/
									{
									    Lmatch[2]--;
										last_state = RP;
										state = END;
									}
									else
									{
										toMatch = '{';
										last_state = UNMATCH;
										state = END;
									}	
								}	
								else if(*current_p == '/')
								{
									if(*(current_p+1) =='*')
									{				
										state = LC;		
									}
									else
									{
										last_state = isSingleOP(current_p);
										state = END;
							    	}
								}
								else if(*current_p == '*')
								{
									if(*(current_p+1) == '/')
									{
										state = RC;
										Comment_Start = NO;
									}
									else
									{
										last_state = isSingleOP(current_p);
										state = END;
									}
								}
							
								
								else
								{
									add = NO;
								}
						break;
						
						/*处理整型*/
						case CONST_INT:
						//printf("CONST_INT\n");
								if( *current_p == '.')   /*发现小数点，转到浮点状态*/
								{
									state = CONST_FLOAT;
								}
						        else if(!isNumber(*current_p)  && !isLetter(*current_p)) /*发现了其他字符或空格*/
								{
									last_state = state;
									state = END;          /*状态结束*/
									current_p--;       /*处理指针回退*/
									add = NO;
								}
						break;	
						
						
						case CONST_FLOAT:
							   if(isLetter(*current_p) || *current_p =='.' )       /*发现了字母或者其他非法结束符*/
								{
									state = ERROR;
								}
						        if(!isNumber(*current_p) ) /*发现了其他字符或空格*/
								{
								
									last_state = state;
									state = END;          /*状态结束*/
									current_p--;       /*处理指针回退*/
									add = NO;
								}								
						break;
						
						
						case CONST_CHAR:
									if(*current_p == 39) /*标识收到了单引号*/
									{
										last_state = state;
										state = END;
									}
									else if(*current_p =='\0')
									{
										last_state = UNMATCH;
									    toMatch = 39;
										state = UNMATCH;
									}
						break;
						
						
						case CONST_STRING:
								if( *current_p == 34)   /*标识收到了结束的双引号*/
								{
									last_state = state;
									state = END;
								}
								else if(*current_p == '\0')
								{
									last_state = UNMATCH;
									toMatch = 34;
									state = UNMATCH;
								}
						break;
						
						
						case IDN:                        
						//printf("IDN\n");
						           if(*current_p == '\0')  /*处理换行情况*/
								   {
										last_state = state;
										state = END;
										add = NO;
										
								   }
						           else if(isLetter(*current_p) == NO && isNumber(*current_p) == NO && *current_p !='_')
								   {
								   //printf("back\n");
										last_state =state;
										state = END;
										current_p--;
										add = NO;
								   }
						break;
						
						
						case GE:
						last_state = state;
						state = END;
						break;


						case LE:
						last_state = state;
						state = END;
						break;


						case EE:
						last_state = state;
						state = END;
						break;
						

						case AND:
						last_state = state;
						state = END;
						break;

						
						case OR:
						last_state = state;
						state = END;
						break;
						
						
						case NE:
						last_state = state;
						state = END;
						break;
						
						case LC:
						last_state = state;
						state = END;
						Lmatch[3]++;
						break;
						
						case RC:
						if(Lmatch[3] >0)
						{
							Lmatch[3]--;
							last_state = state;
							state = END;
							Comment_Start = NO;  /*关闭注释处理*/
						}
						else
						{
							toMatch = 'c';
							last_state = UNMATCH;
							state = END;
						}
						break;
						
						
						case ERROR:
									last_state = state;
									state = END;          /*状态结束*/
									current_p--;       /*处理指针回退*/
									add = NO;

						break;
						
						case UNMATCH:
						break;
						
						
						case END:
						break;
						
						default:
						printf("unknown");
						Token_Error = YES;
						break;
				 }
				 
				 if(add == YES && Comment_Start ==NO)
				 {
					token_str[token_ptr++] = *current_p;
				 }
				 
				 /*保存token串*/
				 if(state == END || state == UNMATCH)
				 {

				    token_str[token_ptr] = '\0';  /*结束当前的token串*/
					token_ptr = 0;
					while(token_str[token_ptr] != '\0')
					printf("%c",token_str[token_ptr++]);
					printf("\t\t\t");				
					printType(last_state,token_str,line_number);
					token_ptr = 0;
					
					state = START;
							
				 }
				 
				 /*已到达行尾，结束，开始下一行*/
			     if(*current_p == '\0')
				 {
				    if(Lmatch[0] > 0 )
					{
						printf("Error: Line:%d Lack \'%c\'  to match \n",line_number,')');
						Token_Error = YES;
						Lmatch[0]--;
					}
					if(Lmatch[1] > 0 )
					{
						printf("Error: Line:%d Lack \'%c\'  to match \n",line_number,']');
						Token_Error = YES;
						Lmatch[1]--;
					}
					break;
				 }
				 current_p++;
             }
			 
			 line_number++;
	
	}
    /*扫描结束*/
	/*源代码文件读取结束*/
	if(Lmatch[2]>0)
	{
	printf("Error: Line:%d Lack \'%c\'  to match \n",line_number,'}');
	Token_Error = YES;
	}
	if(Lmatch[3]>0)
	{
	printf("Error: Line:%d Lack */  to match  comments\n",line_number);
	Token_Error = YES;
	}
	printf("\n----------------------------------\n");
	printf("Source File Scanning finished .\n");
}



  void getToken(char * c)
  {
		
  }
  
  /*函数：判断字符是否为字母*/
   int isLetter(char c)
   {
		if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
		{
				return YES;
		}
		return NO;
   }
   
   
   /*函数：判断字符是否为数字*/
   int isNumber(char c)
   {
		if( c >= '0' && c <= '9')
		{
				return YES;
		}
		return NO;
   }
   
   /*判断是否空格存在*/
   int isBlank(char c)
   {
		if(c == ' ' || c == '\t')
		{
			return YES;
		}
		return NO;
   }
   
   /*打印字符串对应的类型*/
   void printType(int Mode,char *pc,int Line)
   {
    // printf("%d\n",Mode);
		int FLAG = 1;      /*用于判断是否有转义字符*/
		char *toSave = pc;    /*用于保存进入token序列的词素*/
		int keyword_type = 40;  /*用于保存关键字类型*/
        char *p = pc;
		//printf("PRINT%d\n",Mode);
		switch(Mode)
		{
			case CONST_INT:
			while(*p != '\0')
			if(isNumber(*p++) == NO)
			{
				printf("Error:Line:%d illlegal INT or Identifier.\n",Line);
				Token_Error = YES;
			    FLAG = 0;
				break;
			}
			if(FLAG)
			{
				printf("CONST INT\t");
				printf("%d\n",Mode);
				Token = add(Token,toSave,Mode,Line,0,0);  
			}
			break;
			
			
			case CONST_FLOAT:
			printf("CONST FLOAT\t");
			printf("%d\n",Mode);
			Token = add(Token,toSave,Mode,Line,0,0);   /*向字符链表中添加*/
			break;
			
			
			case CONST_CHAR:			
			if( *(p+2)==39)
			{
				printf("CONST CHAR\t");
				printf("%d\n",Mode);
				Token = add(Token,toSave,Mode,Line,0,0);
			}
			else
			printf("Error:Line:%d illegal const char\n",Line);
			Token_Error = YES;
			break;
			
			
			case CONST_STRING:
			while(*p != '\0')
			{
				if(*p++ == 39)
				FLAG = 0;
			}
			if(FLAG)
			{
				printf("STRING\t");
				printf("%d\n",Mode);
				Token = add(Token,toSave,Mode,Line,0,0);
			}
			else 
			printf("Error:Line:%d need to transfer\n",Line);
			Token_Error = YES;
			break;
			
			
			case IDN:
			keyword_type = isKeyword(pc);
			if(keyword_type== INT)
			{
				printf("INT\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}
			else if(keyword_type== FLOAT)
			{
				printf("FLOAT\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}
			else if(keyword_type== CHAR)
			{
				printf("CHAR\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}
			else if(keyword_type== RETURN)
			{
				printf("RETURN\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}        
			else if(keyword_type== WHILE)
			{
				printf("WHILE\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}		
			else if(keyword_type== FOR)
			{
				printf("FOR\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}			
			else if(keyword_type== IF)
			{
				printf("IF\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}
			else if(keyword_type== ELSE)
			{
				printf("ELSE\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}
			else if(keyword_type== VOID)
			{
				printf("VOID\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*属性值默认缺省*/
			}
			else                        /*标识符情况++++++*/
			{
				printf("IDN\t");
				printf("%d\n",Mode);
				Token = add(Token,toSave,Mode,Line,0,0);
			}
			break;
			
			
			case ADD:
				printf("ADD\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/
			break;
			
			
			case SUB:
				printf("SUB\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/			
			break;
			
			
			case SEMI:
				printf("SEMI\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/				
			break;
			
			
			case COM:
				printf("COM\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/				
			break;
			
			
			case DIV:
				printf("DIV\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
			
			
			case GE:
				printf("GE\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;			


			case LE:
				printf("LE\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;



			case EE:
				printf("EE\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
			

			case GT:
				printf("GT\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;


			case LT:
				printf("LT\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;


			case EQ:
				printf("EQ\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
						
			case AND:
				printf("AND\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
			
			case ADDR:
				printf("ADDR\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
			case OR:
				printf("OR\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;


			case ORR:
				printf("ORR\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;

            case NE:
		    	printf("NE\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
            case SLP:
		    	printf("SLP\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;


            case SRP:
		    	printf("SRP\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
	        case ML:
		    	printf("ML\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			

            case MR:
		    	printf("MR\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
            case LP:
		    	printf("LP\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
			
            case RP:
		    	printf("RP\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;
			
			
            case LC:
      			Comment_Start = YES;  /*开始处理注释*/  
		    	printf("LC\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;


            case RC:
		    	printf("RC\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;

            case MUL:
		    	printf("MUL\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*属性值默认缺省*/	
			break;

			
			case UNMATCH:
			Token_Error = YES;
			if(toMatch == 'c')
			printf("Error: Line:%d Lack /*  to match  comments\n",Line);
			else
			printf("Error: Line:%d Lack \'%c\'  to match \n",Line,toMatch);
			break;
			
			case ERROR:
			printf("Error: Line:%d Unknown token. \n",Line);
			Token_Error = YES;
			break;
		}
   }
   
   /*用于判别是否为关键字*/
   int isKeyword(char *c)
   {
		int i ;
		for ( i = 0; i < 9; i++)
		if( strcmp(c,keywords[i]) == 0 )
		return ( i+40);
		
		return NO;
   }
   
   /*用于判别是否为单界特殊符号*/
   int isSingleOP(char *c)
   {
		int i ;
		char cc[2];
        cc[0] =*c;
        cc[1] = '\0';		
		for ( i = 0; i < 30; i++)
		{
			if( strcmp(cc,symbol[i]) == 0 )
				return (i+1);  /*返回具体的符号类型*/
		}
		return NO;
   }
   
   /*判别是否可合成双界符*/
   int isMatchable(char pre,char c)
   {
        /*包含了 +=  -=  ==  :=  != >=  <=*/
		if(pre =='+' || pre == '-' || pre == ':' || pre =='=' || pre=='!'|| pre =='>' || pre == '<')
		{
			if(c == '=')
			return YES;
			else
			return NO;
		}
		else if(pre == '&' && c == '&')
		{
			return YES;
		}
		else if (pre == '|' && c == '|')
		{
			return YES;
		}
		else
		return NO;
   }