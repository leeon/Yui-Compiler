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
#而在rm命令前面加了一个小减 号的意思就是，也许某些文件出现问题，但不要管，继续做后面的事
clean: 
			-rm yui $(OBJ)
