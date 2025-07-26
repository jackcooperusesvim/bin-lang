#define LEXER_H
#ifndef LEXER_H

#include <stdio.h>
#include "stack.h"
#include "str.h"

extern typedef short TokenEnum;

extern const TokenEnum Token_Keyword = 0; //Data: TokenAtString
extern const TokenEnum Token_Identifier = 1; //Data: TokenAtString
extern const TokenEnum Token_Number_Unparsed = 2; //Data: TokenATNumber

extern const TokenEnum Token_CloseParenthesis = 3;
extern const TokenEnum Token_OpenParenthesis = -3;
extern const TokenEnum Token_CloseBracket = 4;
extern const TokenEnum Token_OpenBracket = -4;
extern const TokenEnum Token_CloseAngle = 5;
extern const TokenEnum Token_OpenAngle = -5;

extern const TokenEnum Token_Semicolon = 6;
extern const TokenEnum Token_Comma = 7;
extern const TokenEnum Token_Colon = 8;
extern const TokenEnum Token_Hashtag = 9;
extern const TokenEnum Token_Equals = 10;

extern const TokenEnum Token_Other = 11;
extern const TokenEnum Token_Slash = 12;
extern const TokenEnum Token_Letter = 13;

typedef struct TokenDiscUnion {
	TokenEnum token;
	void* data;
} TokenDiscUnion;

char* read_file(const char* filename, size_t* out_size);
TokenEnum read_char(char c);

#endif
