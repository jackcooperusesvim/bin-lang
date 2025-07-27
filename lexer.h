#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include "stack.h"
#include "str.h"


#define KEYWORD_COUNT 2

typedef short KeywordEnum;
const KeywordEnum Keyword_Comp = 0;
const KeywordEnum Keyword_Print = 1;

typedef short TokenEnum;

//Character Tokens With Data
//Data: short
#define Token_Keyword 0 
//Data: char*
#define Token_Identifier 1 
//Data: long
#define Token_Int 2

//Single Character Encapsulation Tokens
#define Token_CloseParenthesis (TokenEnum) 3
#define Token_OpenParenthesis (TokenEnum) -3

#define Token_CloseBracket (TokenEnum) 4
#define Token_OpenBracket (TokenEnum) -4

#define Token_CloseAngle (TokenEnum) 5
#define Token_OpenAngle (TokenEnum) -5

#define Token_CloseCurly (TokenEnum) -6
#define Token_OpenCurly (TokenEnum) 6

//Single Character Tokens
#define Token_Semicolon (TokenEnum) 18
#define Token_Comma (TokenEnum) 7
#define Token_Colon (TokenEnum) 8
#define Token_Hashtag (TokenEnum) 9
#define Token_Equals (TokenEnum) 10
#define Token_Space (TokenEnum) 11
#define Token_Slash (TokenEnum) 13
#define Token_Carrot (TokenEnum) 19
#define Token_Period (TokenEnum) 20
#define Token_Null_Terminator (TokenEnum) 17
#define Token_Newline (TokenEnum) 15
#define Token_Tab (TokenEnum) 16
#define Token_Nada (TokenEnum) -1

//Data: char*
#define Token_Other (TokenEnum) 12 
#define Token_Letter (TokenEnum) 14



typedef struct TokenDiscUnion {
	TokenEnum token;
	void* data;
} TokenDiscUnion;

char* read_file(const char* filename, size_t* out_size);
TokenEnum read_char(char c);

 

typedef struct TokenStack {
    Stack* token_stack;
    Stack* data_stack;
} TokenStack;

TokenStack* TokenStackNew(size_t tokens);

TokenDiscUnion* TokenStackPush(TokenEnum token_type, size_t len,TokenStack* ts);
void TokenStackRelease(TokenStack* ts) ;
TokenStack* lex_file(const char* file,unsigned long token_count);

typedef int (*LexFn)(char* chars,TokenStack* data);

typedef struct {
    LexFn func;
    TokenStack* t_stack;
} LexClosure;
void PrintTokenDiscUnion(TokenDiscUnion* tdu);

#endif
