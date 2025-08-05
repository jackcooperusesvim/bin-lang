#include "parser.h"
#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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
	instr_stack->bindings = BlockStackNew(cnt,sizeof(Binding));

	return instr_stack;
}
void InstrStackRelease(InstrStack* instr_stack) {
    StackRelease(instr_stack->instructions);
    StackRelease(instr_stack->comp_decls);
    StackRelease(instr_stack->comp_args);
    StackRelease(instr_stack->bindings);
}


void* token_take_and_advance(TokenStack* token_stack, unsigned int* index, TokenEnum match_token) {
    TokenDiscUnion* tok_ptr= BlockStackGetIndPtr(token_stack->token_stack, *index);
    if (tok_ptr->token == match_token) {
	return tok_ptr->data;
    } else {return NULL;}
}

void** parse_tokens(TokenStack* token_stack, unsigned int* index, TokenEnum tokens[static 1], Stack* parser_stack) {
    unsigned int token_cnt = sizeof(tokens) / sizeof(tokens[0]);
    Stack* w_token_stack = token_stack->token_stack;
    void** out = StackPush(parser_stack, token_cnt*sizeof(void**));

    TokenDiscUnion* current_token = BlockStackGetIndPtr(w_token_stack, *index);
    if (current_token == NULL)  {
	perror("confirm_tokens was passed an index beyond the size of the stack");
	exit(1);
    }

    void* end_ptr = w_token_stack->stack_ptr;
    for (int i=0;i<token_cnt;i++) {
	if (current_token == end_ptr) {
	    if (w_token_stack->next== NULL) {perror("Not enough data");exit(1);}
	    w_token_stack = w_token_stack->next;
	    current_token = w_token_stack->data;
	    end_ptr = w_token_stack->stack_ptr;
	}

	if (current_token->token != tokens[i] ) {
	    *index += i;
	    PrintTokenDiscUnion(current_token);
            StackPop(parser_stack, token_cnt*sizeof(void**));
	    return NULL;
	}

        out[i] = current_token->data;

	current_token = current_token+1;
    }
    *index+=token_cnt;
    return (void**) current_token;

}

bool tokens_match(TokenStack* token_stack, unsigned int index, TokenEnum tokens [static 1]) {
    unsigned int token_cnt = sizeof(tokens) / sizeof(tokens[0]);
    Stack* w_token_stack = token_stack->token_stack;
    TokenDiscUnion* current_token = BlockStackGetIndPtr(w_token_stack, index);
    if (current_token == NULL)  {
	perror("confirm_tokens was passed an index beyond the size of the stack");
	exit(1);
    }

    void* end_ptr = w_token_stack->stack_ptr;
    for (int i=0;i<token_cnt;i++) {
	if (current_token == end_ptr) {
	    if (w_token_stack->next== NULL) {perror("Not enough data");exit(1);}
	    w_token_stack = w_token_stack->next;
	    current_token = w_token_stack->data;
	    end_ptr = w_token_stack->stack_ptr;
	}

	if (current_token->token != tokens[i] ) {
	    PrintTokenDiscUnion(current_token);
	    return false;
	}

	current_token = current_token+1;
    }
    return true;
}
PARSE_FUNC_DEF(Binding* , TopLvlBinding) {
    return raw_parse_Binding(token_stack, instr_stack, index, parsing_lifetime_stack, true);
}
PARSE_FUNC_DEF(Binding* , Binding) {
    return raw_parse_Binding(token_stack, instr_stack, index, parsing_lifetime_stack, false);
}

Binding *raw_parse_Binding(TokenStack *token_stack, InstrStack *instr_stack,
                       unsigned int *index, Stack *parsing_lifetime_stack, bool top_level) {
  Binding *out = BlockStackPush(instr_stack->bindings);
  // Component Binding
  if (MATCH_TOKENS(Token_Identifier, Token_OpenAngle)) {
    void **data = PARSE_TOKENS(Token_Identifier, Token_OpenAngle);
    out->ident = (char *)data[0];
    POP_PARSING_STACK(2);

    out->arg_count = 0;
    for (; !MATCH_TOKENS(Token_CloseAngle); out->arg_count++) {
      raw_parse_Binding(token_stack, instr_stack, index, parsing_lifetime_stack, top_level);
    }

    // Variable Binding
    } else if (MATCH_TOKENS(Token_Identifier) && !top_level) {
	out->ident = (char*) *PARSE_TOKENS(Token_Identifier);
	out->arg_count = 0;
	POP_PARSING_STACK(1);

    // Boolean Literal
    } else if (MATCH_TOKENS(Token_Keyword)){
	short kywrd = *(KeywordEnum*)* PARSE_TOKENS(Token_Keyword);
	switch (kywrd) {
	    case Keyword_True:
		out->data = (void*) out;
		break;
	    case Keyword_False:
		out->data = NULL;
		break;
	    default:
		perror("Keywords do not belong in bindings");
		exit(1);
	}
	POP_PARSING_STACK(1);
    }
}

PARSE_FUNC_DEF(CompArg*,DefArgs) {

    CompArg* out = BlockStackPush(instr_stack->comp_args);

    //Component input
    if(MATCH_TOKENS(Token_Keyword,Token_Identifier,Token_OpenAngle)) {

	void** data = PARSE_TOKENS(Token_Keyword,Token_Identifier, Token_OpenAngle);
	if ((*(short*) data[0]) != Keyword_Comp) {perror("Component inputs must use the \"comp\" keyword"); exit(1);}

	out->ident = data[1];


	out->func_arg_count = 0;
	bool funcs_done = false;
	for (;!MATCH_TOKENS(Token_CloseAngle);out->func_arg_count++) {
	    PARSE_FUNC(DefArgs);
	    if (!MATCH_TOKENS(Token_OpenCurly,Token_Int,Token_CloseCurly)) {
		perror("functional component arguments must include a properly formatted cable output (\"{ INT }\") ");
		exit(1);
	    }
	    void** data = PARSE_TOKENS(Token_OpenCurly, Token_Int, Token_CloseCurly);
	    out->wire_count = *(long*) data[1];
	    POP_PARSING_STACK(3);
	}
    }

    //Cable Input
    else if (MATCH_TOKENS(Token_Identifier)) {

	void** data = PARSE_TOKENS(Token_Identifier);
	out->ident = data[0];
	POP_PARSING_STACK(1);

	if (MATCH_TOKENS(Token_Colon,Token_Int)) {

	    data = PARSE_TOKENS(Token_Colon, Token_Int);
	    out->wire_count = *(unsigned int*)data[1];

	    POP_PARSING_STACK(1);
	}

	out->range_end = 0;
	out->func_arg_count = 0;
    }

}
PARSE_FUNC_DEF(CompDecl*,CompDecl) {

    CompDecl* out = BlockStackPush(instr_stack->comp_decls);
    void** data = PARSE_TOKENS(Token_Keyword,Token_Identifier,Token_OpenAngle);

    if (data == NULL) {
	perror("matched characters could not be parsed");
	exit(1);
    }
    out->ident = (char*) data[1];

    POP_PARSING_STACK(3);

    out->arg_count = 0;
    for (;!MATCH_TOKENS(Token_CloseAngle);out->arg_count++) {
	CompArg* arg_data = PARSE_FUNC(DefArgs);
	if (out->arg_count == 0) {out->args = arg_data;}
    }

    if (!MATCH_TOKENS(Token_CloseAngle,Token_OpenCurly)) {
	perror("Component definitions must include them curlies. No prototypes allowed.");
	exit(1);
    }

    PARSE_TOKENS(Token_CloseAngle,Token_OpenCurly);
    POP_PARSING_STACK(2);

    out->sub_decl_count = 0;
    out->binding_count = 0;
    for (;!MATCH_TOKENS(Token_CloseAngle);) {
	if (MATCH_TOKENS(Token_Keyword)) {
	    PARSE_FUNC(CompDecl);
	    out->sub_decl_count++;
	} else if (MATCH_TOKENS(Token_Identifier)){
	    Binding* binding = PARSE_FUNC(Binding);
	    if (out->binding_count == 0 ) {out->bindings = binding;}
	    out->binding_count++;
	} else {
	    perror("Component Definitions can have bindings (starting with an identifier) or sub-components (starting with \"comp\". Nothing else is allowed.");
	    exit(1);
	}
    }

    PARSE_TOKENS(Token_CloseCurly);
    POP_PARSING_STACK(1);

}

PARSE_FUNC_DEF(void,TopLvl) {

    TopLvlInstrDiscUnion* out = BlockStackPush(instr_stack->instructions);

    if (tokens_match(token_stack, *index, (TokenEnum[]){0, 1, (TokenEnum)-5})) {
      out->type = Instr_Comp;
      out->instr = (InstrPtr)PARSE_FUNC(CompDecl);
    }

    if (MATCH_TOKENS(Token_Identifier,Token_OpenAngle)) {
	out->type = Instr_Binding;
	out->instr = (InstrPtr) PARSE_FUNC(Binding);
    } else {
            perror("Non-valid top level line. Must be either a binding or a component declaration");
            exit(1);
    }

}

PARSE_FUNC_DEF(ParsedList, ListCompArg);

void print_InstrStack(InstrStack* instr_stack) {
    unsigned int num_instr = BlockStackCount(instr_stack->instructions, true);
    for (unsigned int i = 0;i<num_instr;i++) {
	TopLvlInstrDiscUnion* instr = BlockStackGetIndPtr(instr_stack->instructions,true);
	// print_TopLvlInstrDiscUnion(instr);
    }
}

int main(void) {

    TokenStack* token_stack = lex_file("input_file.comb", 50000);
    // print_tokenStack(token_stack);
    InstrStack* instr_stack = InstrStackNew(BlockStackCount(token_stack->token_stack,true));
    unsigned int index_val = 0;
    unsigned int* index = 0;
    print_InstrStack(instr_stack);

    Stack* parsing_lifetime_stack = StackNew(1000);
    for (;index_val < BlockStackCount(token_stack->token_stack,true) ;) {
	PARSE_FUNC(TopLvl);
    }

    InstrStackRelease(instr_stack);
}
