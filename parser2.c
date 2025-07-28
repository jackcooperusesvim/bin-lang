// #include "parser.h"
// #include <stddef.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <strings.h>
// #include "lexer.h"
// #include "stack.h"
//
//
// ///---------------------------------------------
// ///---------------------------------------------
// ///CHAT-GPT GENERATED EVERYTHING BELOW THIS LINE
// ///---------------------------------------------
// ///---------------------------------------------
//
// // Forward declarations
// void print_CompArg(CompArg* arg);
// void print_EvalDecl(EvalDecl* eval);
// void print_CompDecl(CompDecl* comp);
// void print_DiscUnionArgIn(DiscUnionArgIn* argin);
// void print_TopLvlInstrDiscUnion(TopLvlInstrDiscUnion* top);
//
void print_CompArg(CompArg* arg) {
    if (!arg) {
        printf("CompArg: NULL\n");
        return;
    }
    printf("CompArg {\n");
    printf("  ident: %s\n", arg->ident);
    printf("  wire_count: %u\n", arg->wire_count);
    printf("  range_end: %u\n", arg->range_end);
    printf("  arg_count: %u\n", arg->arg_count);
    for (unsigned int i = 0; i < arg->arg_count; ++i) {
        printf("  args[%u]:\n", i);
        print_CompArg(&arg->args[i]);
    }
    printf("}\n");
}
//
// // Print EvaluationDecl
// void print_EvalDecl(EvalDecl* eval) {
//     if (!eval) {
//         printf("EvalDecl: NULL\n");
//         return;
//     }
//     printf("EvalDecl {\n");
//     printf("  ident: %s\n", eval->ident);
//     printf("  arg_count: %u\n", eval->arg_count);
//     for (unsigned int i = 0; i < eval->arg_count; ++i) {
//         printf("  args[%u]:\n", i);
//         print_DiscUnionArgIn(&eval->args[i]);
//     }
//     printf("}\n");
// }
//
// // Print ComponentDeclaration
// void print_CompDecl(CompDecl* comp) {
//     if (!comp) {
//         printf("CompDecl: NULL\n");
//         return;
//     }
//     printf("CompDecl {\n");
//     printf("  ident: %s\n", comp->ident);
//     printf("  arg_count: %u\n", comp->arg_count);
//     for (unsigned int i = 0; i < comp->arg_count; ++i) {
//         printf("  args[%u]:\n", i);
//         print_CompArg(&comp->args[i]);
//     }
//     printf("  sub_decl_count: %u\n", comp->sub_decl_count);
//     for (unsigned int i = 0; i < comp->sub_decl_count; ++i) {
//         printf("  sub_decls[%u]:\n", i);
//         print_CompDecl(&comp->sub_decls[i]);
//     }
//     printf("  eval_count: %u\n", comp->eval_count);
//     for (unsigned int i = 0; i < comp->eval_count; ++i) {
//         printf("  evals[%u]:\n", i);
//         print_EvalDecl(&comp->evals[i]);
//     }
//     printf("}\n");
// }
//
// // Print ArgumentInDiscriminatedUnion
// void print_DiscUnionArgIn(DiscUnionArgIn* argin) {
//     if (!argin) {
//         printf("DiscUnionArgIn: NULL\n");
//         return;
//     }
//     printf("DiscUnionArgIn {\n");
//     printf("  type: %u\n", argin->type);
//     switch (argin->type) {
//         case ArgIn_Eval:
//             print_EvalDecl(argin->data.eval);
//             break;
//         case ArgIn_ArgReference:
//             print_CompArg(argin->data.comp_arg);
//             break;
//         case ArgIn_ArgValue:
//             printf("  raw_value: %s\n", argin->data.raw_value ? "true" : "false");
//             break;
//         default:
//             printf("  Unknown ArgInEnum value: %u\n", argin->type);
//             break;
//     }
//     printf("}\n");
// }
//
// // Print TopLvlInstrDiscUnion
// void print_TopLvlInstrDiscUnion(TopLvlInstrDiscUnion* top) {
//     if (!top) {
//         printf("TopLvlInstrDiscUnion: NULL\n");
//         return;
//     }
//     printf("TopLvlInstrDiscUnion {\n");
//     printf("  type: %u\n", top->type);
//     switch (top->type) {
//         case Instr_Comp:
//             printf("  instr.comp_arg:\n");
//             print_CompDecl(top->instr.comp_arg);
//             break;
//         case Instr_Eval:
//             printf("  instr.eval:\n");
//             print_EvalDecl(top->instr.eval);
//             break;
//         case Instr_Print:
//             printf("  Print instruction (no additional data)\n");
//             break;
//         default:
//             printf("  Unknown InstrEnum value: %u\n", top->type);
//             break;
//     }
//     printf("}\n");
// }
// ///END OF CHATGPT
//
// InstrStack* InstrStackNew(size_t token_count) {
// 	InstrStack* instr_stack = (InstrStack*) malloc(sizeof(InstrStack));
// 	size_t cnt = 0;
// 	if (token_count<100) {
// 	    cnt = token_count;
// 	} else {
// 	    cnt = token_count/4;
// 	}
//
//
// 	instr_stack->instructions = BlockStackNew(cnt,sizeof(TopLvlInstrDiscUnion));
// 	instr_stack->comp_decls = BlockStackNew(cnt,sizeof(CompDecl));
// 	instr_stack->comp_args = BlockStackNew(cnt,sizeof(CompArg));
// 	instr_stack->evals = BlockStackNew(cnt,sizeof(EvalDecl));
// 	instr_stack->union_arg_in_decls = BlockStackNew(cnt,sizeof(DiscUnionArgIn));
//
// 	return instr_stack;
// }
// void InstrStackRelease(InstrStack* instr_stack) {
//     StackRelease(instr_stack->instructions);
//     StackRelease(instr_stack->comp_decls);
//     StackRelease(instr_stack->comp_args);
//     StackRelease(instr_stack->evals);
//     StackRelease(instr_stack->union_arg_in_decls);
// }
//
//
// void* token_take_and_advance(TokenStack* token_stack, unsigned int* index, TokenEnum match_token) {
//     TokenDiscUnion* tok_ptr= BlockStackGetIndPtr(token_stack->token_stack, *index);
//     if (tok_ptr->token == match_token) {
// 	return tok_ptr->data;
//     } else {return NULL;}
// }
// // on failure, index is set to the index where it failed
// bool tokens_match(TokenStack* token_stack, unsigned int* index, int token_cnt, TokenEnum *tokens) {
//     Stack* w_token_stack = token_stack->token_stack;
//     TokenDiscUnion* current_token = BlockStackGetIndPtr(w_token_stack, *index);
//     if (current_token == NULL)  {
// 	perror("confirm_tokens was passed an index beyond the size of the stack");
// 	exit(1);
//     }
//
//     void* end_ptr = w_token_stack->stack_ptr;
//     for (int i=0;i<token_cnt;i++) {
// 	if (current_token == end_ptr) {
// 	    if (w_token_stack->next== NULL) {perror("Not enough data");exit(1);}
// 	    w_token_stack = w_token_stack->next;
// 	    current_token = w_token_stack->data;
// 	    end_ptr = w_token_stack->stack_ptr;
// 	}
//
// 	if (current_token->token != tokens[i] ) {
// 	    *index += i;
// 	    PrintTokenDiscUnion(current_token);
// 	    return false;
// 	}
//
// 	current_token = current_token+1;
//     }
//     *index+=token_cnt;
//     return true;
// }
//
// PARSE_FUNC(ParsedList,parseListEval) {
//     ParsedList out;
//
//     out.data = parseEval(token_stack,instr_stack,index);
//     out.count++;
//
//     bool first = true;
//     while(tokens_match(token_stack,index,1,(TokenEnum[]){Token_OpenAngle})) {
//         if (first) {
//             first = false;
//             out.data = parseEval(token_stack,instr_stack,index);
//         } else {
//             parseEval(token_stack,instr_stack,index);
//         }
//         out.count++;
//     }
//     return out;
// }
// PARSE_FUNC(DiscUnionArgIn*,parseDiscUnionArgIn) {
//     DiscUnionArgIn* arg = BlockStackPush(instr_stack->union_arg_in_decls);
//     while(true) {
// 	unsigned int index_cpy = *index;
// 	if(tokens_match(token_stack, &index_cpy, 2, (TokenEnum[]){Token_Identifier,Token_OpenAngle})) {
// 	    arg->type = ArgIn_Eval;
// 	    arg->data = (UnionArgIn) parseEval(token_stack, instr_stack, index);
// 	}
// 	TokenDiscUnion* token = BlockStackAdvanceIndPtr(token_stack->token_stack, index);
// 	if (token->token == Token_Keyword) {
// 	    arg->type = ArgIn_ArgValue;
// 	    TokenEnum tenum = *(TokenEnum*)token->data;
// 	    if (tenum == Keyword_True) {
// 		arg->data = (UnionArgIn) (bool) true;
// 	    } else if (tenum == Keyword_False) {
// 		arg->data = (UnionArgIn) (bool) false;
// 	    }
// 	} else if (token->token == Token_Identifier){
// 	    arg->type = ArgIn_ArgReference;
// 	    UnionArgIn comp_arg = (UnionArgIn) (CompArg*) BlockStackPush(instr_stack->comp_args);
// 	    comp_arg.comp_arg->ident = (char*) token->data;
// 	    if (tokens_match(token_stack,index,1,(TokenEnum[]){Token_Period})) {
// 		long* val = (long*) token_take_and_advance(token_stack, index, Token_Int);
// 		if (val != NULL) { comp_arg.comp_arg->wire_count = *val; }
// 		if (tokens_match(token_stack,index,2,(TokenEnum[]){Token_Period,Token_Period})) { 
// 		    long* val = (long*) token_take_and_advance(token_stack, index, Token_Int);
// 		    if (val != NULL) { comp_arg.comp_arg->range_end= *val; }
// 		}
// 	    } else {
// 		comp_arg.comp_arg->wire_count = 0;
// 	    }
// 	    arg->data =  comp_arg;
// 	}
//
//     }
// }
//
// PARSE_FUNC(EvalDecl*,parseEval) {
//     EvalDecl* out = BlockStackPush(instr_stack->evals);
//     out->ident = (char*) token_take_and_advance(token_stack, index, Token_Identifier);
//     printf("parseEval ident: %s\n",out->ident);
//
//     if (out->ident == NULL) {
// 	perror("Expecting evaluation");
// 	exit(1);
//     }
//     TokenDiscUnion* token = BlockStackGetIndPtr(token_stack->token_stack, *index);
//     printf("parseEval token: %hd\n",token->token);
//     if (!tokens_match(token_stack, index, 1, (TokenEnum[]){Token_OpenAngle})) {
// 	perror("stfu");
// 	exit(1);
//     }
//
//     out->args = BlockStackPush(instr_stack->union_arg_in_decls);
//     while (true) {
// 	if (tokens_match(token_stack, index, 1, (TokenEnum[]){Token_CloseAngle})) {
// 	    break;
// 	}
// 	out->arg_count++;
// 	parseDiscUnionArgIn(token_stack, instr_stack, index);
//     }
//     return out;
// }
//
// PARSE_FUNC(ParsedList,parseListCompDecl) {
//     ParsedList out;
//     CompDecl* comp_decl = BlockStackPush(instr_stack->comp_decls);
//     out.data = comp_decl;
//     while (true) {
// 	CompDecl* comp_decl = BlockStackPush(instr_stack->comp_decls);
//
// 	KeywordEnum* data_ptr = (KeywordEnum*) token_take_and_advance(token_stack, index, Token_Keyword);
// 	if (data_ptr == Keyword_Comp) {
// 		out.count++;
// 		comp_decl= parseCompDecl(token_stack, instr_stack, index);
// 	} else {break;}
// 	comp_decl = BlockStackPush(instr_stack->comp_decls);
//     }
//     return out;
// }
//
// void parseFuncCompArg(TokenStack* token_stack, InstrStack* instr_stack, unsigned int *index,CompArg* out) {
//
//     if (!tokens_match(token_stack, index, 2, (TokenEnum[]){Token_Identifier,Token_OpenAngle} )) {
// 	perror("Bad syntax on function argument declaration");
// 	exit(1);
//     }
//
//     out->ident = (char*) ((TokenDiscUnion*)BlockStackGetIndPtr(token_stack->token_stack, *index-2))->data;
//
//     ParsedList comp_args = parseListCompArg(token_stack,instr_stack,index);
//
//     out->arg_count = comp_args.count;
//     out->args = (CompArg*) comp_args.data;
//
//     //ERROR
//     if (!tokens_match(token_stack, index, 2, (TokenEnum[]){Token_CloseAngle,Token_OpenCurly} )) {
// 	perror("BAD 1");
// 	exit(1);
//     }
//
//     TokenDiscUnion* maybe_wire_count = (TokenDiscUnion*)BlockStackAdvanceIndPtr(token_stack->token_stack, index);
//     if (maybe_wire_count->token == Token_Int) {
// 	out->wire_count = (unsigned int) *(long*)maybe_wire_count->data;
// 	(*index)++;
// 	tokens_match(token_stack, index, 1, (TokenEnum[]){Token_CloseCurly});
//     } else if (maybe_wire_count->token == Token_CloseCurly){
// 	out->wire_count = 0;
// 	(*index)++;
//     } else {
// 	perror("unexpected token found while parsing functional function argument");
// 	exit(1);
//     }
//
//     tokens_match(token_stack, index, 1, (TokenEnum[]){Token_Comma});
// }
//
// PARSE_FUNC(ParsedList,parseListCompArg) {
//     ParsedList out;
//     CompArg* out_data = (CompArg*) BlockStackPush(instr_stack->comp_args);
//     out.data = out_data;
//     unsigned int arg_count;
//     TokenDiscUnion* token;
//
//     bool done_with_funcs;
//
//     printf("parseListCompArg\n");
//     while (true) {
//         token = BlockStackAdvanceIndPtr(token_stack->token_stack, index);
//         printf("%hi\n", token->token);
// 	switch(token->token) {
// 	    case Token_Keyword : {
//                 printf("Token_Keyword\n");
// 		arg_count++;
// 		if ( (*(KeywordEnum*) token->data) != Keyword_Comp ) {
// 			    perror("comp is the only keyword which can be entered during argument declaration");
// 			    exit(1);
// 		}
// 		parseFuncCompArg( token_stack,  instr_stack, index, out_data);
//                 break;
// 	    }
// 	    case Token_Identifier: {
//                 printf("Token_Identifier\n");
// 		arg_count++;
// 		out_data->ident = (char*) token->data;
//                 printf("out_data->ident: %s\n",out_data->ident);
//
// 		//Try for a defined-size input
// 		if (tokens_match(token_stack, index, 1, (TokenEnum[]){Token_Colon})) {
//
// 		    TokenDiscUnion* token2 = BlockStackAdvanceIndPtr(token_stack->token_stack, index);
// 		    if (token2->token == Token_Int) {
// 			out_data->wire_count = *(long*)token2->data;
// 		    } else {
// 			perror("a declaration of a variable input must either be variadic (and last) or have a wire count after a colon");
// 			exit(1);
// 		    }
// 		} else {
// 			perror("variadics are not yet supported");
// 			exit(1);
//                 }
//                 tokens_match(token_stack, index, 1, (TokenEnum[]){Token_Comma});
//                 break;
//
// 	    }
// 	    case Token_CloseAngle: {
//                 printf("Token_CloseAngle\n");
// 		arg_count--;
// 		out.count = arg_count;
//                 printf("out.count: %u\n",out.count);
// 		return out;
// 	    }
// 	    default: {
//                 printf("%hd",token->token);
// 		perror("Something wrong with the separators in your component input declaration");
// 		exit(1);
// 	    }
//
// 	}
// 	out_data = BlockStackPush(instr_stack->comp_args);
//     }
// }
//
// void throwaway(TokenStack* token_stack, InstrStack* instr_stack, unsigned int *index, TokenEnum opener, TokenEnum closer) {
//     TokenDiscUnion* current_ptr;
//     int open_close_state = 1;
//     while (true) {
// 	current_ptr = (TokenDiscUnion*) BlockStackAdvanceIndPtr(token_stack->token_stack, index);
// 	PrintTokenDiscUnion(current_ptr);
// 	if (current_ptr->token == closer) {
// 	    open_close_state-=1;
// 	} else if (current_ptr->token == opener) {
// 	    open_close_state+=1;
// 	}
//
// 	if (open_close_state == 0) {return;}
//     }
// }
//
//
// PARSE_FUNC(CompDecl*,parseCompDecl) {
//     CompDecl* out = BlockStackPush(instr_stack->instructions);
//
//     if (!tokens_match(token_stack, index, 2, (TokenEnum[]){Token_Identifier,Token_OpenAngle} )) {
// 	perror("BAD 2");
// 	exit(1);
//     }
//     (*index)= (*index)-2;
//     out->ident = (char*)((TokenDiscUnion*)BlockStackAdvanceIndPtr(token_stack->token_stack, index))->data;
//
//     (*index)++;
//
//     printf("Parsing arguments\n");
//     ParsedList comp_args = parseListCompArg(token_stack,instr_stack,index);
//
//     out->arg_count = comp_args.count;
//     out->args = (CompArg*) comp_args.data;
//
//     if (!tokens_match(token_stack, index, 1, (TokenEnum[]){Token_OpenCurly} )) {
// 	perror("BAD 3");
// 	exit(1);
//     }
//
//     //GOOD TO HERE
//     printf("Parsing evaluations\n");
//     ParsedList evals = parseListEval(token_stack,instr_stack,index);
//
//     for (int i = 0;i<evals.count;i++) {
//         print_EvalDecl(evals.data);
//     }
//
//     out->eval_count = evals.count;
//     out->evals = (EvalDecl*) evals.data;
//
//     if (!tokens_match(token_stack, index, 1, (TokenEnum[]){Token_CloseCurly} )) {
// 	perror("Component declaration must end with a '}' ");
// 	exit(1);
//     }
//
//     return out;
// }
//
// PARSE_FUNC(void,parseTopLevel) {
//
//     TokenDiscUnion* first_token = BlockStackGetIndPtr(token_stack->token_stack,*index);
//     TopLvlInstrDiscUnion* out = BlockStackPush(instr_stack->instructions);
//
//     switch (first_token->token) {
// 	case Token_Keyword : {
// 	    (*index)++;
// 	    switch (*(KeywordEnum*) first_token->data) {
// 		case Keyword_Comp : {
// 		    out->type = Instr_Comp;
// 		    CompDecl* comp_decl = parseCompDecl(token_stack,instr_stack,index);
// 		    out->instr = (InstrPtr) comp_decl;
// 		    break;
// 		}
// 		case Keyword_Print : {
// 		    out->type = Instr_Print;
// 		    (*index)++;
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
// 	    out->type = Instr_Eval;
// 	    EvalDecl* eval = parseEval(token_stack,instr_stack,index);
// 	    out->instr = (InstrPtr) (EvalDecl*) eval;
// 	}
//     }
// }
//
// void parse(TokenStack *token_stack, InstrStack *instr_stack) {
//     unsigned int index = 0;
//     while (true) {
// 	parseTopLevel(token_stack, instr_stack, &index);
//     }
// }
//
//
// void print_InstrStack(InstrStack* instr_stack) {
//     unsigned int num_instr = BlockStackCount(instr_stack->instructions, true);
//     for (unsigned int i = 0;i<num_instr;i++) {
// 	TopLvlInstrDiscUnion* instr = BlockStackGetIndPtr(instr_stack->instructions,true);
// 	print_TopLvlInstrDiscUnion(instr);
//     }
// }
//
// int main(void) {
//
//     TokenStack* token_stack = lex_file("input_file.comb", 50000);
//     // print_tokenStack(token_stack);
//     InstrStack* instr_stack = InstrStackNew(BlockStackCount(token_stack->token_stack,true));
//
//     parse(token_stack, instr_stack);
//     print_InstrStack(instr_stack);
//
//     InstrStackRelease(instr_stack);
// }

