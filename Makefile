main: main.c stack.c base.c var.c env.c
	gcc main.c stack.c base.c var.c env.c -o exec

lexer: lexer.c stack.c
	gcc lexer.c stack.c -o lexer
