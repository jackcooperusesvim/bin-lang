main: main.c stack.c base.c var.c env.c
	gcc main.c stack.c base.c var.c env.c -g -o exec

lexer_demo: lexer_demo.c lexer.c stack.c str.c
	gcc lexer_demo.c lexer.c stack.c str.c -g -O0 -o lexer_demo

parser: parser.c stack.c str.c lexer.c
	gcc parser.c lexer.c stack.c str.c -g -O0 -o parser

interpreter: interpreter.c stack.c str.c lexer.c
	gcc interpreter.c lexer.c stack.c str.c -g -O0 -o interpreter
