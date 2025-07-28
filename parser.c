#include "parser.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "lexer.h"
#include "stack.h"

// Forward declarations
void print_CompArg(CompArg* arg);
void print_EvalDecl(EvalDecl* eval);
void print_CompDecl(CompDecl* comp);
void print_DiscUnionArgIn(DiscUnionArgIn* argin);
void print_TopLvlInstrDiscUnion(TopLvlInstrDiscUnion* top);

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
	instr_stack->evals = BlockStackNew(cnt,sizeof(EvalDecl));
	instr_stack->union_arg_in_decls = BlockStackNew(cnt,sizeof(DiscUnionArgIn));

	return instr_stack;
}
void InstrStackRelease(InstrStack* instr_stack) {
    StackRelease(instr_stack->instructions);
    StackRelease(instr_stack->comp_decls);
    StackRelease(instr_stack->comp_args);
    StackRelease(instr_stack->evals);
    StackRelease(instr_stack->union_arg_in_decls);
}

ParsedList parse_ListCompArg(TokenStack *token_stack, InstrStack *instr_stack,
                             unsigned int *index,TokenEnum sep_token) {

    ParsedList out;
    CompArg* out_data = (CompArg*) BlockStackPush(instr_stack->comp_args);
    out.data = out_data;

    while (true) {
        TokenDiscUnion* token = BlockStackAdvanceIndPtr(token_stack->token_stack, index);

        if (token->token != Token_Identifier) {
            break;
        }

        out.count++;
        out_data->ident = (char*) token->data;
        token = BlockStackAdvanceIndPtr(token_stack->token_stack, index);
        if (token->token != sep_token) {
            perror("Something wrong with the separators in your component input declaration");
            exit(1);
        }
        token = BlockStackAdvanceIndPtr(token_stack->token_stack, index);
        if (token->token != Token_Int) {
            perror("Something wrong with the separators in your component input declaration");
            exit(1);
        }
        out_data->wire_count = *(long*)token->data;

        out_data = (CompArg*) BlockStackPush(instr_stack->comp_args);
    }
    return out;
}

int main(void) {
    TokenStack* token_stack = lex_file("input_file.comb", 50000);
    print_tokenStack(token_stack);
    InstrStack* instr_stack = InstrStackNew(BlockStackCount(token_stack->token_stack,true));

    ParsedList out = parse_ListCompArg(token_stack, instr_stack,0,Token_Colon);
    for (int i = 0;i<out.count;i++) {
        print_CompArg(out.data);
    }

    TokenStackRelease(token_stack);
    InstrStackRelease(instr_stack);
}
