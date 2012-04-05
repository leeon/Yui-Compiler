#ifndef STATE_H
#define STATE_H
/*dfa״̬��*/

#define START 0 
#define CONST_INT 1
#define CONST_FLOAT 2
#define CONST_CHAR 3     /*���ܵ����ź�*/
#define CONST_STRING 4  /*����˫���ź�*/
#define IDN 5         /*��ʶ��*/
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
#define LC 23         /*��ע�ͷ���*/
#define RC 24        /*��ע�ͷ���*/
#define AND 25      /* &&*/
#define OR 26       /* || */
#define EE 27             /* == */
#define COM 28          /*����*/
#define ADDR 29    /*  &  */
#define ORR 30      /*  |  */



/* 

 �ķ�����  EXPRESSION
��1���ʽ�ķ�   ����id+id*id��
              E->TE1          {E.id = T.id  E.addr = T.addr ;gencode(E.addr,E1.addr)}
              E1->=TE1    { E1.addr :=  newtemp} emit( E1.addr ,'=',p_T.addr,OP,T) 
			  E1 -> ~        {�ڲ��ж�}
              T->FT1         { T.id :=  F.id}{T.addr = T1.addr}{T.type = F1.type �ж�}
              T1->+FT1	{T1.addr := newtemp; emit( T1.addr ,'=',p_F.addr,OP,F.addr) }
			                         {widen(p_F.type,F.type)}			  
			  T1 -> ~
			  F->MF1        { F.id := M.id}{F.addr = F1.addr}{F.type = F1.type �ж�}
			  F1-> *MF1   { F1.addr := newtemp; emit(F1.addr,'=',p_M.addr,OP, M.addr) }
			                         {widen(p_M.type,M.type)}
			  F1-> ~
              M	->  (E)     { M.id := E.id}{M.addr := E.addr} {M.type := E.type} 
			  M-> id         {p := lookup(id.name);  M.type = id.type
		                             if p ?  then M.id =p  else error }

 �ķ�����
��2 IF ELSE�ķ� ��
			S�� if /gencode; backpatch(goto boole);/(E)  {MATCH} /gencode;/S1
			S1��~|else /backpatch(goto false)/{MATCH} /backpatch(goto end); /
	    	MATCH->  E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH  |~

int temp_order = 0;

int addr_order = 1000;

 �ķ����� ASSIGN
 (3��ֵ���ʽ�ķ�)
			A ->  type id {checkBlock; addTable(id);} B1 B2 ;
			B1 -> =EB1 | ~
			B2 -> , id{checkBlock; addTable(id);} B1 B2 |~


 �ķ����� FUNCTION 
 (4�������������ķ�)
		FUN -> {gencode;}type id {checkBlock; addTable(id);} (P)D1
		D1 -> { MATCH}
		MATCH ->  E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH  |~
		

 �ķ����� WHILE���
 ��5 whileѭ���ķ�������
		 W -> {gencode}while(E)W1{backpatch(goto false);backpatch(goto loop)}
		 W1 ->{ MATCH }
		 MATCH -> E MATCH| S MATCH | A MATCH | W MATCH |~

		 
 �ķ����� FOR���
  ��6 forѭ���ķ�������
		FR -> {gencode}for(E E {backpatch(end)}E{backpatch(goto bool)}) FR1{backpatch(goto step)}
		FR1 -> {MATCH}
		MATCH -> E MATCH| S MATCH | A MATCH | W MATCH | FR MATCH |~


 �ķ����� �����б�
 ��7 �����б��ķ�������
		P ->type id {checkBlock; addTable(id);}  P1
		P1 -> , type id {checkBlock; addTable(id);} P1 |~
		
*/


/*�ؼ��ֲ���  �жϵ�ʱ��ע�����40�Ļ���*/
#define INT 40
#define FLOAT 41
#define CHAR 42
#define RETURN 43
#define WHILE 44
#define FOR 45
#define IF 46
#define ELSE 47
#define VOID 48



/*��ע�������*/


#define RETURN_EXPRESSION 84
#define FUN_BLOCK 85
#define FOR_BLOCK 86
#define WHILE_BLOCK 87
#define ASSIGN 88
#define IF_ELSE 89
#define EXPRESSION 90



#define UNMATCH 98   /*�������ŵȱ�ʶ����ƥ�������*/
#define ERROR 99
#define END 100


/*common*/
#define NEXT 1114
#define BLANK 1113
#define VN 1112
#define YES 1111
#define NO 0
#define MORE 110   /*���ڱ�ע���������Ƿ����չ*/

#endif