#include "parser.h"
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


void* token_take_and_advance(TokenStack* token_stack, unsigned int* index, TokenEnum match_token) {
    TokenDiscUnion* tok_ptr= BlockStackGetIndPtr(token_stack->token_stack, *index);
    if (tok_ptr->token == match_token) {
	return tok_ptr->data;
    } else {return NULL;}
}
// on failure, index is set to the index where it failed
bool tokens_match(TokenStack* token_stack, unsigned int* index, int token_cnt, TokenEnum *tokens) {
    Stack* w_token_stack = token_stack->token_stack;
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
	    return false;
	}

	current_token = current_token+1;
    }
    *index+=token_cnt;
    return true;
}

//UNFINISHED PARSING FUNCTIONS
PARSE_FUNC(ParsedList,parseListEval) {
    while(true) {
    }
}
PARSE_FUNC(DiscUnionArgIn*,parseDiscUnionArgIn) {
    ParsedList out;
    DiscUnionArgIn* arg = BlockStackPush(instr_stack->union_arg_in_decls);
    out.data = arg;
    while(true) {
	unsigned int index_cpy = *index;
	if(tokens_match(token_stack, &index_cpy, 2, (TokenEnum[]){Token_Identifier,Token_OpenAngle})) {
	    arg->type = ArgIn_Eval;
	    arg->data = (UnionArgIn) parseEval(token_stack, instr_stack, index);
	}
	TokenDiscUnion* token = BlockStackAdvanceIndPtr(token_stack->token_stack, index);
	if (token->token == Token_Keyword) {
	    arg->type = ArgIn_ArgValue;
	    TokenEnum tenum = *(TokenEnum*)token->data;
	    if (tenum == Keyword_True) {
		arg->data = (UnionArgIn) (bool) true;
	    } else if (tenum == Keyword_False) {
		arg->data = (UnionArgIn) (bool) false;
	    }
	} else if (token->token == Token_Identifier){
	    arg->type = ArgIn_ArgReference;
	    UnionArgIn comp_arg = (UnionArgIn) (CompArg*) BlockStackPush(instr_stack->comp_args);
	    comp_arg.comp_arg->ident = (char*) token->data;
	    if (tokens_match(token_stack,index,1,(TokenEnum[]){Token_Period})) {
		long* val = (long*) token_take_and_advance(token_stack, index, Token_Int);
		if (val != NULL) { comp_arg.comp_arg->wire_count = *val; }
		if (tokens_match(token_stack,index,2,(TokenEnum[]){Token_Period,Token_Period})) { 
		    long* val = (long*) token_take_and_advance(token_stack, index, Token_Int);
		    if (val != NULL) { comp_arg.comp_arg->range_end= *val; }
		}
	    } else {
		comp_arg.comp_arg->wire_count = 0;
	    }
	    arg->data =  comp_arg;
	}

    }
}

PARSE_FUNC(EvalDecl*,parseEval) {
    EvalDecl* out = BlockStackPush(instr_stack->evals);
    out->ident = (char*) token_take_and_advance(token_stack, index, Token_Identifier);

    if (out->ident == NULL) {
	perror("Expecting evaluation");
	exit(1);
    }
    if (!tokens_match(token_stack, index, 1, (TokenEnum[]){Token_OpenAngle})) {
	perror("idk, man");
	exit(1);
    }
 
    out->args = BlockStackPush(instr_stack->union_arg_in_decls);
    while (true) {
	if (tokens_match(token_stack, index, 1, (TokenEnum[]){Token_CloseAngle})) {
	    break;
	}
	out->arg_count++;
	parseDiscUnionArgIn(token_stack, instr_stack, index);
    }
    return out;
}

PARSE_FUNC(ParsedList,parseListCompDecl) {
    ParsedList out;
    CompDecl* comp_decl = BlockStackPush(instr_stack->comp_decls);
    out.data = comp_decl;
    while (true) {
	CompDecl* comp_decl = BlockStackPush(instr_stack->comp_decls);

	KeywordEnum* data_ptr = (KeywordEnum*) token_take_and_advance(token_stack, index, Token_Keyword);
	if (data_ptr == Keyword_Comp) {
		out.count++;
		comp_decl= parseCompDecl(token_stack, instr_stack, index);
	} else {break;}
	comp_decl = BlockStackPush(instr_stack->comp_decls);
    }
    return out;
}

void parseFuncCompArg(TokenStack* token_stack, InstrStack* instr_stack, unsigned int *index,CompArg* out) {

    if (!tokens_match(token_stack, index, 2, (TokenEnum[]){Token_Identifier,Token_OpenAngle} )) {
	perror("Bad syntax on function argument declaration");
	exit(1);
    }

    out->ident = (char*) ((TokenDiscUnion*)BlockStackGetIndPtr(token_stack->token_stack, *index-2))->data;

    ParsedList comp_args = parseListCompArg(token_stack,instr_stack,index);

    out->arg_count = comp_args.count;
    out->args = (CompArg*) comp_args.data;

    //compargs instead of garbage
    if (!tokens_match(token_stack, index, 2, (TokenEnum[]){Token_CloseAngle,Token_OpenCurly} )) {
	perror("BAD 1");
	exit(1);
    }

    TokenDiscUnion* maybe_wire_count = (TokenDiscUnion*)BlockStackAdvanceIndPtr(token_stack->token_stack, index);
    if (maybe_wire_count->token == Token_Int) {
	out->wire_count = (unsigned int) *(long*)maybe_wire_count->data;
	(*index)++;
	tokens_match(token_stack, index, 1, (TokenEnum[]){Token_CloseCurly});
    } else if (maybe_wire_count->token == Token_CloseCurly){
	out->wire_count = 0;
	(*index)++;
    } else {
	perror("unexpected token found while parsing functional function argument");
	exit(1);
    }

    tokens_match(token_stack, index, 1, (TokenEnum[]){Token_Comma});
}

PARSE_FUNC(ParsedList,parseListCompArg) {
    ParsedList out;
    CompArg* out_data = (CompArg*) BlockStackPush(instr_stack->comp_args);
    out.data = out_data;
    unsigned int arg_count;
    TokenDiscUnion* first_token = BlockStackAdvanceIndPtr(token_stack->token_stack, index);

    bool variadic_encountered;
    bool done_with_funcs;

    while (true) {
	switch(first_token->token) {
	    case Token_Keyword : {
		arg_count++;
		if ( (*(KeywordEnum*) first_token->data) != Keyword_Comp ) {
			    perror("comp is the only keyword which can be entered during argument declaration");
			    exit(1);
		}
		parseFuncCompArg( token_stack,  instr_stack, index, out_data);
	    }
	    case Token_Identifier: {
		arg_count++;
		if (variadic_encountered) {
		    perror("variadic inputs must always come last");
		    exit(1);
		}
		out_data->ident = (char*) first_token->data;

		//Try for a defined-size input
		if (tokens_match(token_stack, index, 1, (TokenEnum[]){Token_Colon})) {

		    TokenDiscUnion* token = BlockStackAdvanceIndPtr(token_stack->token_stack, index);
		    if (token->token == Token_Int) {
			out_data->wire_count = *(long*)token->data;
		    } else {
			perror("a declaration of a variable input must either be variadic (and last) or have a wire count after a colon");
			exit(1);
		    }
		    tokens_match(token_stack, index, 1, (TokenEnum[]){Token_Comma});
		} else {
		}

	    }
	    case Token_CloseAngle: {
		arg_count--;
		out.count = arg_count;
		return out;
	    }
	    default: {
		perror("Something wrong with the separators in your component input declaration");
		exit(1);
	    }

	}
	out_data = BlockStackPush(instr_stack->comp_args);
    }
}

void throwaway(TokenStack* token_stack, InstrStack* instr_stack, unsigned int *index, TokenEnum opener, TokenEnum closer) {
    TokenDiscUnion* current_ptr;
    int open_close_state = 1;
    while (true) {
	current_ptr = (TokenDiscUnion*) BlockStackAdvanceIndPtr(token_stack->token_stack, index);
	PrintTokenDiscUnion(current_ptr);
	if (current_ptr->token == closer) {
	    open_close_state-=1;
	} else if (current_ptr->token == opener) {
	    open_close_state+=1;
	}

	if (open_close_state == 0) {return;}
    }
}


PARSE_FUNC(CompDecl*,parseCompDecl) {
    CompDecl* out = BlockStackPush(instr_stack->instructions);

    if (!tokens_match(token_stack, index, 2, (TokenEnum[]){Token_Identifier,Token_OpenAngle} )) {
	perror("BAD 1");
	exit(1);
    }
    out->ident = (char*)((TokenDiscUnion*)BlockStackAdvanceIndPtr(token_stack->token_stack, index))->data;

    ParsedList comp_args = parseListCompArg(token_stack,instr_stack,index);

    out->arg_count = comp_args.count;
    out->args = (CompArg*) comp_args.data;

    //compargs instead of garbage
    if (!tokens_match(token_stack, index, 2, (TokenEnum[]){Token_CloseAngle,Token_OpenCurly} )) {
	perror("BAD 1");
	exit(1);
    }

    ParsedList comp_decls = parseListCompDecl(token_stack,instr_stack,index);
    ParsedList evals = parseListEval(token_stack,instr_stack,index);

    out->sub_decl_count = comp_decls.count;
    out->sub_decls = (CompDecl*) evals.data;

    out->eval_count = evals.count;
    out->evals = (EvalDecl*) evals.data;

    if (!tokens_match(token_stack, index, 1, (TokenEnum[]){Token_CloseCurly} )) {
	perror("Component declaration must end with a '}' ");
	exit(1);
    }

    return out;
}

PARSE_FUNC(void,parseTopLevel) {

    TokenDiscUnion* first_token = BlockStackGetIndPtr(token_stack->token_stack,*index);
    TopLvlInstrDiscUnion* out = BlockStackPush(instr_stack->instructions);

    switch (first_token->token) {
	case Token_Keyword : {
	    (*index)++;
	    switch (*(KeywordEnum*) first_token->data) {
		case Keyword_Comp : {
		    out->type = Instr_Comp;
		    CompDecl* comp_decl = parseCompDecl(token_stack,instr_stack,index);
		    out->instr = (InstrPtr) comp_decl;
		    break;
		}
		case Keyword_Print : {
		    out->type = Instr_Print;
		    (*index)++;
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
	    out->type = Instr_Eval;
	    EvalDecl* eval = parseEval(token_stack,instr_stack,index);
	    out->instr = (InstrPtr) (EvalDecl*) eval;
	}
    }
}

void parse(TokenStack *token_stack, InstrStack *instr_stack) {
    unsigned int index = 0;
    while (true) {
	parseTopLevel(token_stack, instr_stack, &index);
    }
}


void print_InstrStack(InstrStack* instr_stack) {
    unsigned int num_instr = BlockStackCount(instr_stack->instructions, true);
    for (unsigned int i = 0;i<num_instr) {
	TopLvlInstrDiscUnion* instr = BlockStackGetIndPtr(instr_stack->instructions,true);
    }

    instr_stack->instructions
}

int main(void) {

    TokenStack* token_stack = lex_file("input_file.comb", 50000);
    // print_tokenStack(token_stack);
    InstrStack* instr_stack = InstrStackNew(BlockStackCount(token_stack->token_stack,true));

    parse(token_stack, instr_stack, 0);

    InstrStackRelease(instr_stack);
}

