#ifndef STATE_H
#define STATE_H
/*dfa状态表*/

#define START 0 
#define CONST_INT 1
#define CONST_FLOAT 2
#define CONST_CHAR 3     /*接受单引号后*/
#define CONST_STRING 4  /*接受双引号后*/
#define IDN 5         /*标识符*/
#define LP 6           /* { */
#define RP 7          /* } */
#define ADD 8      /*+*/
#define SUB 9
#define DIV 10
#define MUL 11    /*  *  */
#define EQ 12        /*  =  */
#define GT 13       /*>*/
#define LT 14        /*<*/
#define SEMI 15   /* ; */
#define SLP 16      /* ( */
#define SRP 17      /* ) */
#define NE 18        /* != */
#define ML 19       /* [ */
#define MR 20       /* ] */
#define LE 21       /* <= */
#define GE 22      /* >= */
#define LC 23         /*左注释符号*/
#define RC 24        /*右注释符号*/
#define AND 25      /* &&*/
#define OR 26       /* || */
#define EE 27             /* == */
#define COM 28          /*逗号*/
#define ADDR 29    /*  &  */
#define ORR 30      /*  |  */



/* 

 文法描述  EXPRESSION
（1表达式文法   形如id+id*id）
              E->TE1          {E.id = T.id  E.addr = T.addr ;gencode(E.addr,E1.addr)}
              E1->=TE1    { E1.addr :=  newtemp} emit( E1.addr ,'=',p_T.addr,OP,T) 
			  E1 -> ~        {内部判断}
              T->FT1         { T.id :=  F.id}{T.addr = T1.addr}{T.type = F1.type 判断}
              T1->+FT1	{T1.addr := newtemp; emit( T1.addr ,'=',p_F.addr,OP,F.addr) }
			                         {widen(p_F.type,F.type)}			  
			  T1 -> ~
			  F->MF1        { F.id := M.id}{F.addr = F1.addr}{F.type = F1.type 判断}
			  F1-> *MF1   { F1.addr := newtemp; emit(F1.addr,'=',p_M.addr,OP, M.addr) }
			                         {widen(p_M.type,M.type)}
			  F1-> ~
              M	->  (E)     { M.id := E.id}{M.addr := E.addr} {M.type := E.type} 
			  M-> id         {p := lookup(id.name);  M.type = id.type
		                             if p ?  then M.id =p  else error }

 文法描述
（2 IF ELSE文法 ）
			S→ if /gencode; backpatch(goto boole);/(E)  {MATCH} /gencode;/S1
			S1→~|else /backpatch(goto false)/{MATCH} /backpatch(goto end); /
	    	MATCH->  E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH  |~

int temp_order = 0;

int addr_order = 1000;

 文法描述 ASSIGN
 (3赋值表达式文法)
			A ->  type id {checkBlock; addTable(id);} B1 B2 ;
			B1 -> =EB1 | ~
			B2 -> , id{checkBlock; addTable(id);} B1 B2 |~


 文法描述 FUNCTION 
 (4函数声明描述文法)
		FUN -> {gencode;}type id {checkBlock; addTable(id);} (P)D1
		D1 -> { MATCH}
		MATCH ->  E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH  |~
		

 文法描述 WHILE语句
 （5 while循环文法描述）
		 W -> {gencode}while(E)W1{backpatch(goto false);backpatch(goto loop)}
		 W1 ->{ MATCH }
		 MATCH -> E MATCH| S MATCH | A MATCH | W MATCH |~

		 
 文法描述 FOR语句
  （6 for循环文法描述）
		FR -> {gencode}for(E E {backpatch(end)}E{backpatch(goto bool)}) FR1{backpatch(goto step)}
		FR1 -> {MATCH}
		MATCH -> E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH |~


 文法描述 参数列表
 （7 参数列表文法描述）
		P ->type id {checkBlock; addTable(id);}  P1
		P1 -> , type id {checkBlock; addTable(id);} P1 |~
		
*/


/*关键字部分  判断的时候注意加上40的基础*/
#define INT 40
#define FLOAT 41
#define CHAR 42
#define RETURN 43
#define WHILE 44
#define FOR 45
#define IF 46
#define ELSE 47
#define VOID 48



/*标注语义分析*/


#define RETURN_EXPRESSION 84
#define FUN_BLOCK 85
#define FOR_BLOCK 86
#define WHILE_BLOCK 87
#define ASSIGN 88
#define IF_ELSE 89
#define EXPRESSION 90



#define UNMATCH 98   /*处理引号等标识符不匹配的问题*/
#define ERROR 99
#define END 100


/*common*/
#define NEXT 1114
#define BLANK 1113
#define VN 1112
#define YES 1111
#define NO 0
#define MORE 110   /*用于标注，操作符是否可扩展*/

#endif