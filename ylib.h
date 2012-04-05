#ifndef YLIB_H
#define YLIB_H
/*
* ylib.h
*@author octobershiner
*@version 0.1
* 2011 11 25
*/

   
   /*�˵�*/
   void hello();
   void help();
   /*Tag the error in Token Scanner*/
	int Token_Error;
	int Syntax_Error;
	int Sem_Error;


/**********************************�ʷ�����********************************************/
/*
���ű�ṹ��
value �ֱ���
name ����
line ���ڱ����кţ�������token����
block_num ���ڱ������ڵ�����
type���� id������
*/
struct Node
{
	int value;
	int line;
	int block_num;
	char name[30];
	struct Node* next;
	int type;
};


extern struct Node * Table;  /*���ű����*/
extern struct Node * Token;   /*token����*/
extern struct CodeNode * Code;  /*�м����*/

/*to tell keywords and operators quickly */
extern char keywords[30][30];
extern char symbol[40][30];


/*�ʷ�ɨ�躯��*/
   void scanner(char * file_name);
   int isLetter(char c);
   int isNumber(char c);
   int isBlank(char c);
   void printType(int MODE,char *pc,int Line);
   int isKeyword(char *c);
   int isSingleOP(char *c);
   int isMatchable(char pre,char c);
   
  /*����Ĳ���*/
   struct Node * initTable(struct Node*h);
   struct Node* add(struct Node*h,char * p,int VALUE,int LINE,int BLOCK,int type);
   void printToken(struct Node* h);
   void writeTokenFile(struct Node* h);
   
   
/**********************************�﷨����********************************************/ 


/*
	�﷨�����������ݽṹ
	order ���ڵ����
	value ���ս����ΪVT �ս����Ϊ��������
	name ��ӡ�﷨��������ʱ��ʹ��
	children ���溢�ӽ��
	parent ���游�ڵ�
	num ʵ�ʺ�����
*/
struct TreeNode
{
	int order;  
	int value;
	char name[30];
	struct TreeNode* children[15];  /*�����ʽ��10������*/
	struct TreeNode* parent;
	int num;
};


extern struct TreeNode * SyntaxTree;

/*���Ĳ���*/
struct TreeNode* initTree(struct TreeNode* h);
struct TreeNode* addTree(struct TreeNode* currentTree,int p_order,int p_value,char* p_name);
void traverse(struct TreeNode* h,FILE* gf);
void graphviz();
  
/*�﷨ɨ�躯��*/
  void parser();  
  void init_buffer();
  
  

 /**********************************�������********************************************/

/*blockջ*/
#define STACK_SIZE 30
extern int Stack[STACK_SIZE];
extern int *top;
extern int *bottom;
/*ջ�Ĳ���*/
void initStack();
int pop();
void push( int num);
int searchStack(int num); 
 
 
/*
�м�������ݽṹ
addr��ʾ��ǰ���е�ַ��
goto��ʾҪ��ת���е�ַ�ţ���������תĬ��ΪNEXT
*/
struct CodeNode
{
	int addr;
	char sentence[200];
	int _goto;
	struct CodeNode *next;
};

/*to name all the temp variable for the assemble language*/ 
extern int temp_order;
/*initialize the code*/
struct CodeNode *initCode();
/*add node to tha ADT of the code*/
struct CodeNode* addCode(struct CodeNode* h,char* sentence,int addr,int _goto);
/*generate the assignment code*/
struct CodeNode *gencode(struct CodeNode* h,int addr,char* LEFT,char*OP,char* RIGHT);
/*generate the return code*/
struct CodeNode* genReturnCode(struct CodeNode* h,int addr,char* p_addr);
/*generate the goto code*/
struct CodeNode* genGotoCode(struct CodeNode* h,int addr);
/*deal with the backpatch*/
void editCode(struct CodeNode* h,int goto_addr);
/*generate the function code*/
struct CodeNode* genFunCode(struct CodeNode* h,int addr,char* name);
/*generate the if_goto code*/
struct CodeNode* genControlCode(struct CodeNode* h,int  bool_e,int addr);
/*generate the 3address code*/
struct CodeNode *emit(struct CodeNode* h,int addr,char*RESULT, char* OP1, char*LEFT,char*OP2, char*RIGHT);
/*print the code to the screen*/
void printCode(struct CodeNode* h);
/*write file *.s */
void writeCode(struct CodeNode* h);
/*do the type conversion*/
int widen(int left,int right,int line);

/*�������������*/
struct Link
{
	char addr[20];
	char id[20];
	int type;
};

#endif