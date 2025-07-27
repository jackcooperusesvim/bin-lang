main: main.c stack.c base.c var.c env.c
	gcc main.c stack.c base.c var.c env.c -g -o exec

lexer: lexer_test.c lexer.c stack.c str.c
	gcc lexer_test.c lexer.c stack.c str.c -g -O0 -o lexer

parser: parser.c stack.c str.c lexer.c
	gcc parser.c lexer.c stack.c str.c -g -O0 -o parser
