#include "parser.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"
#include "stack.h"

InstrStack* InstrStackNew(size_t token_count) {
	InstrStack* instr_stack = (InstrStack*) malloc(sizeof(InstrStack));
	size_t cnt = 0;
	if (token_count<100) {
	    cnt = token_count;
	} else {
	    cnt = token_count/4;
	}


	instr_stack->instructions = BlockStackNew(cnt,sizeof(TopLvlInstrDiscUnion));
	instr_stack->comp_decls = BlockStackNew(cnt,sizeof(CompDecl));
	instr_stack->comp_args = BlockStackNew(cnt,sizeof(CompArg));
	instr_stack->evals = BlockStackNew(cnt,sizeof(Eval));
	instr_stack->union_arg_in_decls = BlockStackNew(cnt,sizeof(UnionArgIn));

	return instr_stack;
}
void InstrStackRelease(InstrStack* instr_stack) {
    StackRelease(instr_stack->instructions);
    StackRelease(instr_stack->comp_decls);
    StackRelease(instr_stack->comp_args);
    StackRelease(instr_stack->evals);
    StackRelease(instr_stack->union_arg_in_decls);
}

ParsedSingle parseEval(TokenStack* token_stack, InstrStack* instr_stack, unsigned int index);

const TokenEnum CompDeclExpTokenLists[][3] = {
    {Token_Identifier,Token_OpenAngle,Token_Nada},
    {Token_CloseAngle,Token_Equals,Token_OpenCurly},
    {Token_CloseCurly,Token_Semicolon,Token_Nada}
};

// on failure, index is set to the index where it failed
bool confirm_tokens(TokenStack* token_stack, unsigned int* index, int token_cnt, TokenEnum *tokens) {
    Stack* w_token_stack = token_stack->token_stack;
    TokenDiscUnion* current_token = BlockStackGetIndPtr(w_token_stack, *index);
    if (current_token == NULL)  {
	perror("confirm_tokens was passed an index beyond the size of the stack");
	exit(1);
    }

    void* end_ptr = w_token_stack->stack_ptr;
    for (int i=0;i<token_cnt;i++) {
	bool reset = false;
	if (current_token == end_ptr) {
	    reset = true;
	    if (w_token_stack->next== NULL) {perror("Not enough data");exit(1);}
	    w_token_stack = w_token_stack->next;
	    current_token = w_token_stack->data;
	    end_ptr = w_token_stack->stack_ptr;
	}

	if (current_token->token != tokens[i] ) {
	    *index = i;
	    PrintTokenDiscUnion(current_token);
	    return false;
	}

	current_token = current_token+1;
    }
    return true;
}

// ParsedSingle parseCompDecl(TokenStack* token_stack, InstrStack* instr_stack, unsigned int index) {
//     TokenDiscUnion* working_token = StackGetUniform(token_stack->token_stack, sizeof(TokenDiscUnion),index);
//     CompDecl* out_data = BlockStackPush(instr_stack->instructions);
//     ParsedSingle out;
//     out.data = (void*) out_data;
//
//     while (working_token->token != Token_Semicolon) {
//     }
// }

// ParsedSingle parseTopLevel(TokenStack* token_stack, InstrStack* instr_stack, unsigned int index) {
//
//     TokenDiscUnion* first_token = StackGetUniform(token_stack->token_stack,sizeof(TokenDiscUnion),index);
//     TopLvlInstrDiscUnion* out_data = BlockStackPush(instr_stack->instructions);
//     ParsedSingle out;
//     out.data = (void*) out_data;
//
//     switch (first_token->token) {
// 	case Token_Keyword : {
// 	    switch (*(KeywordEnum*) first_token->data) {
// 		case Keyword_Comp : {
// 		    out_data->type = Instr_Comp;
// 		    // ParsedSingle comp_decl = parseCompDecl(token_stack,instr_stack,index+1);
// 		    // out_data->instr = (InstrPtr) (CompDecl*) comp_decl.data;
// 		    // out.next_index = comp_decl.next_index;
// 		    break;
// 		}
// 		case Keyword_Print : {
// 		    out_data->type = Instr_Print;
// 		    out.next_index = index+1;
// 		    break;
// 		}
// 		default: {
// 		    perror("Wrong keyword. Expecting 'comp', 'print', or an evaluation");
// 		    PrintTokenDiscUnion(first_token);
// 		    exit(1);
// 		}
// 	    }
// 	    break;
// 	}
// 	case Token_Identifier : {
// 	    out_data->type = Instr_Eval;
// 	    ParsedSingle eval = parseEval(token_stack,instr_stack,index);
// 	    out.next_index = eval.next_index;
// 	    out_data->instr = (InstrPtr) (Eval*) eval.data;
// 	}
//     }
//     return out;
// }

int main(void) {

    TokenStack* token_stack = lex_file("input_file.comb", 50000);
    print_tokenStack(token_stack);

    unsigned int index = 0;
    bool confirmation = confirm_tokens(token_stack, &index, 3, (TokenEnum[]){Token_Keyword,Token_Identifier,Token_OpenAngle});

    printf("%i\n",confirmation);

    InstrStack* instr_stack = InstrStackNew(StackCountUniform(token_stack->token_stack,true));

    TokenStackRelease(token_stack);

    InstrStackRelease(instr_stack);
}

