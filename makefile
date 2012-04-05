# octobershiner's first makefile
OBJ = Yui.o Scanner.o Table.o lib.o SyntaxTree.o Syntax.o Gen_code.o

Token : $(OBJ)
			gcc -g -o yui $(OBJ)
Yui.o : ylib.h state.h
Scanner.o :  state.h ylib.h 
Table.o :  state.h ylib.h
SyntaxTree.o : state.h ylib.h
Syntax.o : state.h ylib.h
Gen_code.o	:state.h ylib.h 
.PHONY : clean		
#����rm����ǰ�����һ��С�� �ŵ���˼���ǣ�Ҳ��ĳЩ�ļ��������⣬����Ҫ�ܣ��������������
clean: 
			-rm yui $(OBJ)
