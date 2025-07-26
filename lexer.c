#include "lexer.h"
#include "stack.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

const int KEYWORD_COUNT = 1;
const char* KEYWORDS[KEYWORD_COUNT] =  {"let\0"};


typedef short TokenEnum;

const TokenEnum Token_Keyword = 0; //Data: TokenAtString
const TokenEnum Token_Identifier = 1; //Data: TokenAtString
const TokenEnum Token_Number_Unparsed = 2; //Data: TokenATNumber

const TokenEnum Token_CloseParenthesis = 3;
const TokenEnum Token_OpenParenthesis = -3;
const TokenEnum Token_CloseBracket = 4;
const TokenEnum Token_OpenBracket = -4;
const TokenEnum Token_CloseAngle = 5;
const TokenEnum Token_OpenAngle = -5;

const TokenEnum Token_Semicolon = 6;
const TokenEnum Token_Comma = 7;
const TokenEnum Token_Colon = 8;
const TokenEnum Token_Hashtag = 9;
const TokenEnum Token_Equals = 10;
const TokenEnum Token_Space= 11;

const TokenEnum Token_Other = 12;
const TokenEnum Token_Slash = 13;
const TokenEnum Token_Letter = 14;
const TokenEnum Token_Newline = 15;
const TokenEnum Token_Null_Terminator= 16;
const TokenEnum Token_Nada= -1;


TokenEnum read_char(char c) {
    switch (c) {
        case '(': return Token_OpenParenthesis;
        case ')': return Token_CloseParenthesis;
        case '[': return Token_OpenBracket;
        case ']': return Token_CloseBracket;
        case '<': return Token_OpenAngle;
        case '>': return Token_CloseAngle;
        case ';': return Token_Semicolon;
        case ',': return Token_Comma;
        case ':': return Token_Colon;
        case '#': return Token_Hashtag;
        case '=': return Token_Equals;
        case '/': return Token_Slash;
        case '\n': return Token_Newline;
        case ' ': return Token_Space;
        case '\0': return Token_Null_Terminator;

        // Letters (both uppercase and lowercase)
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
            return Token_Letter;

        // Digits
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return Token_Number_Unparsed;

        default:
            return Token_Other;
    }
}

typedef struct TokenDiscUnion {
	TokenEnum token;
	void* data;
} TokenDiscUnion;

//Associated Types

typedef struct TokenATString {
	unsigned int len;
	char* characters;
} TokenATString;

typedef struct TokenATNumber{
	unsigned long int num;
} TokenATid;

typedef struct TokenStack {
    Stack token_stack;
    Stack data_stack;
} TokenStack;

TokenStack* TokenStackNew(size_t tokens) {
    TokenStack* ts = malloc(sizeof(TokenStack));

    StackNewHere(tokens*sizeof(TokenDiscUnion),&ts->token_stack);
    StackNewHere(tokens*50,&ts->data_stack);
    return ts;
}

TokenDiscUnion* TokenStackPush(TokenEnum token_type, size_t len,TokenStack* ts) {
    TokenDiscUnion* token = StackPush(&ts->token_stack, sizeof(TokenDiscUnion));
    char* str = StackPush(&ts->data_stack, len);
    token->data = str;
    return token;
}


typedef int (*ParseFn)(char* chars,TokenStack** data);

typedef struct {
    ParseFn func;
    void* env;
} ParseClosure;
 
#define MAKE_CLOSURE(fn, ctx) ((ParseClosure){ .func = fn, .env = ctx })

int forEachLine(const char* restrict filename, ParseClosure closure) {
    FILE * file = fopen("input_file.comb", "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    char* line = NULL;
    size_t len = 0;

    while (getline(&line, &len, file) != -1) {
        int err = closure.func(line,closure.env);
        if (err != 0 ) {
            return err;
        }
    }

    free(line);
    fclose(file);
    return 0;
}

int println(char* c) {
    printf("%s",c);
    return 0;
}

void FinishToken(TokenEnum token_type, char* buf, int first_char, int last_char, TokenStack* stack) {
    if (first_char == -1) {return;}

    TokenDiscUnion* token = StackPush(&stack->token_stack,sizeof(TokenEnum));
    switch(token_type) {
        case Token_Letter: {
            char* str =  my_strcopy(&buf[first_char],last_char-first_char);
            for (short i = 0; i<KEYWORD_COUNT;i++) {
                if (my_strcmp((char*) KEYWORDS[i],str)) {
                    token->token = Token_Keyword;
                    token->data = StackPush(&stack->data_stack, sizeof(short));
                    *(short*)token->data = i;
                    return;
                }
            }
            token->token = Token_Identifier;
            token->data = StackPush(&stack->data_stack, sizeof(char)*(last_char-first_char));
            return;
        }
        //TODO: FINISH THE OTHER CASES
    }

}

int lex_line(char* line, TokenStack** stack) {
    size_t count = 0;
    for (; line[count] != *"\0"; count++);

    *stack = (TokenStack*) TokenStackNew(count*(sizeof(TokenDiscUnion)+sizeof(TokenATString)+100));


    //Start Lexing
    TokenEnum current_type = Token_Nada;
    int token_start_ind = 0;
    int token_last_ind=1;

    for (;line[token_last_ind] != *"\0"; token_last_ind++) {
        TokenEnum type = read_char(line[token_last_ind]);
        if (type != current_type)  {
            FinishToken(type, line,token_start_ind,token_last_ind,*stack);
            token_start_ind = token_last_ind;
            current_type = type;
        }
    }
    //Lex the last token
    FinishToken(current_type, line,token_start_ind,token_last_ind,*stack);
    return 0;
}

int main(void) {

    TokenStack ** tokenstack_mut_ptr = NULL;
    ParseClosure c = MAKE_CLOSURE(lex_line,*tokenstack_mut_ptr)
    forEachLine("input_file.comb",println);
}
