#include "lexer.h"
#include "stack.h"
#include "str.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

const char* KEYWORDS[KEYWORD_COUNT] =  {"comp","print"};



typedef short TokenEnum;


TokenEnum read_char(char c) {
    switch (c) {
        case '(': return Token_OpenParenthesis;
        case ')': return Token_CloseParenthesis;
        case '{': return Token_OpenCurly;
        case '^': return Token_Carrot;
        case '}': return Token_CloseCurly;
        case '[': return Token_OpenBracket;
        case ']': return Token_CloseBracket;
        case '<': return Token_OpenAngle;
        case '>': return Token_CloseAngle;
        case ';': return Token_Semicolon;
        case ',': return Token_Comma;
        case '.': return Token_Period;
        case ':': return Token_Colon;
        case '#': return Token_Hashtag;
        case '=': return Token_Equals;
        case '/': return Token_Slash;

        case '\n': return Token_Newline;
        case '\t': return Token_Tab;
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
            return Token_Int;

        default:
            return Token_Other;
    }
}

void PrintTokenDiscUnion(TokenDiscUnion* tdu) {
    switch (tdu->token) {
        case Token_OpenParenthesis: printf("(\n"); break;
        case Token_CloseParenthesis: printf(")\n"); break;
        case Token_OpenCurly: printf("{\n"); break;
        case Token_CloseCurly: printf("}\n"); break;
        case Token_OpenBracket: printf("[\n"); break;
        case Token_CloseBracket: printf("]\n"); break;
        case Token_OpenAngle: printf("<\n"); break;
        case Token_Carrot: printf("^\n"); break;
        case Token_CloseAngle: printf(">\n"); break;
        case Token_Semicolon: printf(";\n"); break;
        case Token_Comma: printf(",\n"); break;
        case Token_Period: printf(".\n"); break;
        case Token_Colon: printf(":\n"); break;
        case Token_Hashtag: printf("#\n"); break;
        case Token_Equals: printf("=\n"); break;
        case Token_Slash: printf("/\n"); break;
        case Token_Newline: printf("NEWLINE\n"); break;
        case Token_Tab: printf("TAB\n"); break;
        case Token_Space: printf("SPACE\n"); break;

        // For these, printf a representative character.
        case Token_Keyword: {
            printf("Keyword\n");
            printf("\tdata: %hd",*(short*) tdu->data);
            break;
        };

        case Token_Identifier: {
            printf("Identifier\n");
            printf("\tdata: %s",(char*) tdu->data);
            break;
        };

        case Token_Int: {
            printf("Integer\n");
            printf("\tdata: %ld",*(long*) tdu->data);
            break;
        };

        case Token_Other: {
            printf("Other\n");
            printf("\tdata: %s",(char*) tdu->data);
            break;
        };

        default:
            printf("Unknown");  // Unknown or unsupported token
            break;
    }
    printf("\n");
}


TokenStack* TokenStackNew(size_t tokens) {
    TokenStack* ts = malloc(sizeof(TokenStack));

    ts->token_stack = BlockStackNew(tokens,sizeof(TokenDiscUnion));
    ts->data_stack = StackNew(tokens*50);
    return ts;
}

TokenDiscUnion* TokenStackPush(TokenEnum token_type, size_t len,TokenStack* ts) {
    TokenDiscUnion* token = BlockStackPush(ts->token_stack);
    char* str = StackPush(ts->data_stack, len);
    token->data = str;
    return token;
}
void TokenStackRelease(TokenStack* ts) {
    StackRelease(ts->token_stack);
    StackRelease(ts->data_stack);
    free(ts);
}


int forEachLine(const char* restrict filename, LexClosure closure) {
    FILE * file = fopen("input_file.comb", "r");
    if (!file) {
        perror("fopen");
        return 1;
    }
    char* line = NULL;
    size_t len = 0;

    while (getline(&line, &len, file) != -1) {
        int err = closure.func(line,closure.t_stack);
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
    int buf_len = last_char-first_char;
    if (first_char == -1) {return;}

    TokenDiscUnion* token = BlockStackPush(stack->token_stack);
    // printf("%p\n",token);
    // printf("%li\n",( token - (TokenDiscUnion*) stack->token_stack->data));
    switch(token_type) {
        case Token_Letter: {
            char* str = my_strcopy((buf)+first_char,buf_len);
            //Check if the letter is a keyword
            for (short i = 0; i<KEYWORD_COUNT;i++) {
                if (my_strcmp((char*) KEYWORDS[i],str)) {
                    token->data = StackPush(stack->data_stack, sizeof(short));
                    token->token = Token_Keyword ;
                    *(short*)token->data = i;
                    free(str);
                    return;
                }
            }
            token->token = Token_Identifier;
            token->data = my_strcopy_stack(str,stack->data_stack);
            free(str);
            return;
        }
        case Token_Int: {
            char* str =  my_strcopy(buf+first_char,buf_len);
            token->token = Token_Int;
            token->data = StackPush(stack->data_stack, sizeof(long));
            *(long*)token->data = atol(str);
            free(str);
            return;
        }
        case Token_Tab:
        case Token_Newline:
        case Token_Null_Terminator:
        case Token_Space: {
            if (BlockStackPop(stack->token_stack) != 0) {
                perror("Last Stack Doesn't have that a full block allocated");
                exit(1);
            }
            return;
        }
        case Token_Other: {
            char* str =  my_strcopy(&buf[first_char],buf_len);
            token->data = my_strcopy_stack(str, stack->data_stack);
            token->token = Token_Other;
        }
        default: {
            token->token = token_type;
            for (int i = buf_len-1;i>0;i--) {
                TokenDiscUnion * token_ptr = BlockStackPush(stack->token_stack);
                token_ptr->token = token_type;
            }
        }
    }

}

void print_tokenStack(TokenStack* stack) {
    TokenDiscUnion* stack_ptr = (TokenDiscUnion*) stack->token_stack->stack_ptr;
    TokenDiscUnion* start_ptr = (TokenDiscUnion*) stack->token_stack->data;

    ptrdiff_t num_tokens = stack_ptr-start_ptr;
    for (int i = 0; i<num_tokens;i++) {
        PrintTokenDiscUnion((TokenDiscUnion*)&start_ptr[i]);
    }
    if (stack->token_stack->next != NULL) {
        print_tokenStack(stack);
    }
}

int lex_line(char* line, TokenStack* stack) {
    size_t count = 0;
    for (; line[count] != *"\0"; count++);

    //Start Lexing
    TokenEnum current_type = read_char(line[0]);

    int token_start_ind = 0;
    int token_last_ind=1;

    for (;line[token_last_ind] != *"\0"; token_last_ind++) {
        TokenEnum type = read_char(line[token_last_ind]);
        if (type != current_type)  {
            // printf("%s\n",(line+token_start_ind));
            FinishToken(current_type, line,token_start_ind,token_last_ind,stack);
            token_start_ind = token_last_ind;
            current_type = type;
        }
    }
    //Lex the last token
    FinishToken(current_type, line,token_start_ind,token_last_ind,stack);

    return 0;
}

TokenStack* lex_file(const char* file,unsigned long token_count) {
    TokenStack * tokenstack_ptr = TokenStackNew(token_count);
    LexClosure c = {.func = lex_line, .t_stack = tokenstack_ptr};
    forEachLine("input_file.comb",c);
    return tokenstack_ptr;
}

int main(void) {
    printf("lexing...\n");
    TokenStack* ts = lex_file("input_file.comb", 5000000);
    printf("printing...\n");
    print_tokenStack(ts);
    TokenStackRelease(ts);
}
