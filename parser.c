#include "parser.h"
#include <stddef.h>
#include <stdarg.h>
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

bool confirm_tokens(TokenStack* token_stack, unsigned int* index, int token_cnt, ...) {
    Stack* w_token_stack = token_stack->token_stack;
    va_list list;
    va_start(list,token_cnt);
    TokenDiscUnion* current_token = BlockStackGetIndPtr(w_token_stack, *index);
    void* end_ptr = StackEndPtr(w_token_stack, current_token);

    for (int i=0;i<token_cnt;i++) {
	if (current_token == end_ptr) {
	    current_token = w_token_stack->data;
	    end_ptr = StackEndPtr(w_token_stack, current_token);
	}
	if (current_token->token != (TokenEnum) va_arg(list,int)) {
	    return false;
	}
    }
    return true;
}

ParsedSingle parseCompDecl(TokenStack* token_stack, InstrStack* instr_stack, unsigned int index) {
    TokenDiscUnion* working_token = StackGetUniform(token_stack->token_stack, sizeof(TokenDiscUnion),index);
    CompDecl* out_data = BlockStackPush(instr_stack->instructions);
    ParsedSingle out;
    out.data = (void*) out_data;

    while (working_token->token != Token_Semicolon) {
    }
}

ParsedSingle parseTopLevel(TokenStack* token_stack, InstrStack* instr_stack, unsigned int index) {

    TokenDiscUnion* first_token = StackGetUniform(token_stack->token_stack,sizeof(TokenDiscUnion),index);
    TopLvlInstrDiscUnion* out_data = BlockStackPush(instr_stack->instructions);
    ParsedSingle out;
    out.data = (void*) out_data;

    switch (first_token->token) {
	case Token_Keyword : {
	    switch (*(KeywordEnum*) first_token->data) {
		case Keyword_Comp : {
		    out_data->type = Instr_Comp;
		    ParsedSingle comp_decl = parseCompDecl(token_stack,instr_stack,index+1);
		    out_data->instr = (InstrPtr) (CompDecl*) comp_decl.data;
		    out.next_index = comp_decl.next_index;
		    break;
		}
		case Keyword_Print : {
		    out_data->type = Instr_Print;
		    out.next_index = index+1;
		    break;
		}
		default: {
		    perror("Wrong keyword. Expecting 'comp', 'print', or an evaluation");
		    PrintTokenDiscUnion(first_token);
		    exit(1);
		}
	    }
	    break;
	}
	case Token_Identifier : {
	    out_data->type = Instr_Eval;
	    ParsedSingle eval = parseEval(token_stack,instr_stack,index);
	    out.next_index = eval.next_index;
	    out_data->instr = (InstrPtr) (Eval*) eval.data;
	}
    }
    return out;
}

int main(void) {

    TokenStack* token_stack = lex_file("input_file.comb", 50000);

    unsigned int token_count = StackCountUniform(token_stack->token_stack,sizeof(TokenDiscUnion),true);
    InstrStack* instr_stack = InstrStackNew(token_count);

    TokenStackRelease(token_stack);

    InstrStackRelease(instr_stack);
}

