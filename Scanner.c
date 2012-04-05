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

/*���ڱ�����֤ƥ������  Ĭ��Ϊ˫����*/
char toMatch = 34; 

/*���ڱ�� ����ƥ����� ����Ϊ ( ) [] {} ע�ͷ�*/
int Lmatch[4] ={0,0,0,0};

/*���ڼ�¼�Ƿ�ʼע��*/
int Comment_Start = NO;

/*
*ɨ�躯��
*/
												 
void scanner(char * file_name)
{
	 char current_line[MAX_LINE];       /*�洢ÿһ�е��ַ���*/ 
	 char * current_p = NULL;               /*��ǰ�ַ�ָ��*/
	 int line_number = 1;                                                                                                                                                                                                     
	 
     char token_str[TOKEN_SIZE];    /*���ڴ洢ÿһ��token��*/
	 int token_ptr = 0;  /*ָ��ǰtoken��β*/
	 
	 int last_state = START;  /*���ڱ����һ��token��������*/
	 int state = START;
	 
	 int add = YES;

	  
     /*��ʼ ��ȡԴ�����ļ�*/
     FILE * fp = NULL;
	 if ( ( fp = fopen(file_name,"r") ) == NULL)  
	 {
			printf("YuiToken->Error: There is no such file.\n");
			Token_Error = YES;
			exit(0);
	 } 
	printf("YuiToken->Success: Source File loaded successfully!\n");
	
	
	/*ɨ������е�ÿһ��*/
	 while(fgets(current_line,MAX_LINE,fp) != 0)
	 {
			printf("\nLine %d : \n",line_number);
			 current_p = current_line;                   /*���õ�ǰָ��*/
			 while(1)
			 {
			  
			     add = YES;
				 switch(state)
				 {
						case START:
						         /*����հ��ַ�*/
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
								else if (*current_p ==39)/*�����˵�����*/
								{
									state = CONST_CHAR;
								}
								else if(*current_p == 34)  /*������˫����*/
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
									if(Lmatch[0] > 0)  /*�ж�ǰ���Ƿ��������ſ�ƥ��*/
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
									if(Lmatch[1] > 0)  /*�ж�ǰ���Ƿ����������ſ�ƥ��*/
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
									if(Lmatch[2] > 0)  /*�ж�ǰ���Ƿ���������ſ�ƥ��*/
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
						
						/*��������*/
						case CONST_INT:
						//printf("CONST_INT\n");
								if( *current_p == '.')   /*����С���㣬ת������״̬*/
								{
									state = CONST_FLOAT;
								}
						        else if(!isNumber(*current_p)  && !isLetter(*current_p)) /*�����������ַ���ո�*/
								{
									last_state = state;
									state = END;          /*״̬����*/
									current_p--;       /*����ָ�����*/
									add = NO;
								}
						break;	
						
						
						case CONST_FLOAT:
							   if(isLetter(*current_p) || *current_p =='.' )       /*��������ĸ���������Ƿ�������*/
								{
									state = ERROR;
								}
						        if(!isNumber(*current_p) ) /*�����������ַ���ո�*/
								{
								
									last_state = state;
									state = END;          /*״̬����*/
									current_p--;       /*����ָ�����*/
									add = NO;
								}								
						break;
						
						
						case CONST_CHAR:
									if(*current_p == 39) /*��ʶ�յ��˵�����*/
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
								if( *current_p == 34)   /*��ʶ�յ��˽�����˫����*/
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
						           if(*current_p == '\0')  /*���������*/
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
							Comment_Start = NO;  /*�ر�ע�ʹ���*/
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
									state = END;          /*״̬����*/
									current_p--;       /*����ָ�����*/
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
				 
				 /*����token��*/
				 if(state == END || state == UNMATCH)
				 {

				    token_str[token_ptr] = '\0';  /*������ǰ��token��*/
					token_ptr = 0;
					while(token_str[token_ptr] != '\0')
					printf("%c",token_str[token_ptr++]);
					printf("\t\t\t");				
					printType(last_state,token_str,line_number);
					token_ptr = 0;
					
					state = START;
							
				 }
				 
				 /*�ѵ�����β����������ʼ��һ��*/
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
    /*ɨ�����*/
	/*Դ�����ļ���ȡ����*/
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
  
  /*�������ж��ַ��Ƿ�Ϊ��ĸ*/
   int isLetter(char c)
   {
		if( (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') )
		{
				return YES;
		}
		return NO;
   }
   
   
   /*�������ж��ַ��Ƿ�Ϊ����*/
   int isNumber(char c)
   {
		if( c >= '0' && c <= '9')
		{
				return YES;
		}
		return NO;
   }
   
   /*�ж��Ƿ�ո����*/
   int isBlank(char c)
   {
		if(c == ' ' || c == '\t')
		{
			return YES;
		}
		return NO;
   }
   
   /*��ӡ�ַ�����Ӧ������*/
   void printType(int Mode,char *pc,int Line)
   {
    // printf("%d\n",Mode);
		int FLAG = 1;      /*�����ж��Ƿ���ת���ַ�*/
		char *toSave = pc;    /*���ڱ������token���еĴ���*/
		int keyword_type = 40;  /*���ڱ���ؼ�������*/
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
			Token = add(Token,toSave,Mode,Line,0,0);   /*���ַ����������*/
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
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}
			else if(keyword_type== FLOAT)
			{
				printf("FLOAT\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}
			else if(keyword_type== CHAR)
			{
				printf("CHAR\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}
			else if(keyword_type== RETURN)
			{
				printf("RETURN\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}        
			else if(keyword_type== WHILE)
			{
				printf("WHILE\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}		
			else if(keyword_type== FOR)
			{
				printf("FOR\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}			
			else if(keyword_type== IF)
			{
				printf("IF\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}
			else if(keyword_type== ELSE)
			{
				printf("ELSE\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}
			else if(keyword_type== VOID)
			{
				printf("VOID\t%d\n",keyword_type);
				Token = add(Token,"",keyword_type,Line,0,0);  /*����ֵĬ��ȱʡ*/
			}
			else                        /*��ʶ�����++++++*/
			{
				printf("IDN\t");
				printf("%d\n",Mode);
				Token = add(Token,toSave,Mode,Line,0,0);
			}
			break;
			
			
			case ADD:
				printf("ADD\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/
			break;
			
			
			case SUB:
				printf("SUB\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/			
			break;
			
			
			case SEMI:
				printf("SEMI\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/				
			break;
			
			
			case COM:
				printf("COM\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/				
			break;
			
			
			case DIV:
				printf("DIV\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
			
			
			case GE:
				printf("GE\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;			


			case LE:
				printf("LE\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;



			case EE:
				printf("EE\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
			

			case GT:
				printf("GT\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;


			case LT:
				printf("LT\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;


			case EQ:
				printf("EQ\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
						
			case AND:
				printf("AND\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
			
			case ADDR:
				printf("ADDR\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
			case OR:
				printf("OR\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;


			case ORR:
				printf("ORR\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;

            case NE:
		    	printf("NE\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
            case SLP:
		    	printf("SLP\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;


            case SRP:
		    	printf("SRP\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
	        case ML:
		    	printf("ML\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			

            case MR:
		    	printf("MR\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
            case LP:
		    	printf("LP\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
			
            case RP:
		    	printf("RP\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;
			
			
            case LC:
      			Comment_Start = YES;  /*��ʼ����ע��*/  
		    	printf("LC\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;


            case RC:
		    	printf("RC\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
			break;

            case MUL:
		    	printf("MUL\t%d\n",Mode);
				Token = add(Token,"",Mode,Line,0,0);  /*����ֵĬ��ȱʡ*/	
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
   
   /*�����б��Ƿ�Ϊ�ؼ���*/
   int isKeyword(char *c)
   {
		int i ;
		for ( i = 0; i < 9; i++)
		if( strcmp(c,keywords[i]) == 0 )
		return ( i+40);
		
		return NO;
   }
   
   /*�����б��Ƿ�Ϊ�����������*/
   int isSingleOP(char *c)
   {
		int i ;
		char cc[2];
        cc[0] =*c;
        cc[1] = '\0';		
		for ( i = 0; i < 30; i++)
		{
			if( strcmp(cc,symbol[i]) == 0 )
				return (i+1);  /*���ؾ���ķ�������*/
		}
		return NO;
   }
   
   /*�б��Ƿ�ɺϳ�˫���*/
   int isMatchable(char pre,char c)
   {
        /*������ +=  -=  ==  :=  != >=  <=*/
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