#ifndef YLIB_H
#define YLIB_H
/*
* ylib.h
*@author octobershiner
*@version 0.1
* 2011 11 25
*/

   
   /*菜单*/
   void hello();
   void help();
   /*Tag the error in Token Scanner*/
	int Token_Error;
	int Syntax_Error;
	int Sem_Error;


/**********************************词法分析********************************************/
/*
符号表结构体
value 种别码
name 属性
line 用于保存行号，仅限于token序列
block_num 用于保存所在的语句块
type保存 id的类型
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


extern struct Node * Table;  /*符号表入口*/
extern struct Node * Token;   /*token序列*/
extern struct CodeNode * Code;  /*中间代码*/

/*to tell keywords and operators quickly */
extern char keywords[30][30];
extern char symbol[40][30];


/*词法扫描函数*/
   void scanner(char * file_name);
   int isLetter(char c);
   int isNumber(char c);
   int isBlank(char c);
   void printType(int MODE,char *pc,int Line);
   int isKeyword(char *c);
   int isSingleOP(char *c);
   int isMatchable(char pre,char c);
   
  /*链表的操作*/
   struct Node * initTable(struct Node*h);
   struct Node* add(struct Node*h,char * p,int VALUE,int LINE,int BLOCK,int type);
   void printToken(struct Node* h);
   void writeTokenFile(struct Node* h);
   
   
/**********************************语法分析********************************************/ 


/*
	语法分析树的数据结构
	order 树节点序号
	value 非终结符号为VT 终结符则为具体类型
	name 打印语法分析树的时候使用
	children 保存孩子结点
	parent 保存父节点
	num 实际孩子数
*/
struct TreeNode
{
	int order;  
	int value;
	char name[30];
	struct TreeNode* children[15];  /*最长产生式有10个孩子*/
	struct TreeNode* parent;
	int num;
};


extern struct TreeNode * SyntaxTree;

/*树的操作*/
struct TreeNode* initTree(struct TreeNode* h);
struct TreeNode* addTree(struct TreeNode* currentTree,int p_order,int p_value,char* p_name);
void traverse(struct TreeNode* h,FILE* gf);
void graphviz();
  
/*语法扫描函数*/
  void parser();  
  void init_buffer();
  
  

 /**********************************语义分析********************************************/

/*block栈*/
#define STACK_SIZE 30
extern int Stack[STACK_SIZE];
extern int *top;
extern int *bottom;
/*栈的操作*/
void initStack();
int pop();
void push( int num);
int searchStack(int num); 
 
 
/*
中间代码数据结构
addr表示当前的行地址号
goto表示要跳转的行地址号，无特殊跳转默认为NEXT
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

/*语义分析数据链*/
struct Link
{
	char addr[20];
	char id[20];
	int type;
};

#endif