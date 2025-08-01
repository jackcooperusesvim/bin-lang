#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"
#include "stack.h"

typedef struct ArgumentInDiscriminatedUnion DiscUnionArgIn;
typedef unsigned short ArgInEnum;
typedef unsigned char InstrEnum;
typedef struct ComponentDeclaration CompDecl;

#define Instr_Comp 0
#define Instr_Binding 1

#define ArgIn_Binding 0
#define ArgIn_ArgReference 1
#define ArgIn_ArgValue 2
 
#define PARSE_FUNC_DEF(ret_type, name) ret_type parse_##name(TokenStack* token_stack, InstrStack* instr_stack, unsigned int *index, Stack* parsing_lifetime_stack)
#define PARSE_FUNC(name) parse_##name( token_stack,  instr_stack, index, parsing_lifetime_stack)

#define MATCH_TOKENS(tokens...) tokens_match(token_stack,*index,(TokenEnum[]){tokens})
#define PARSE_TOKENS(tokens...) parse_tokens(token_stack,index,(TokenEnum[]){tokens},parsing_lifetime_stack)
#define POP_PARSING_STACK(num_tokens) StackPop(parsing_lifetime_stack,num_tokens*sizeof(void**))

//wire_count of 0 means variadic
typedef struct ComponentArgument {
	char* ident;
	unsigned int wire_count; //Also range_start
	unsigned int range_end;
	
	//For function arguments
	unsigned int func_arg_count;
	//function arg args are next in the stack
} CompArg;

typedef struct EvaluationDecl {
	char* ident;

	unsigned int arg_count;
	DiscUnionArgIn* args;

} Binding;

//is_print discriminates between a print statment (which contains no more data
typedef struct ComponentDeclaration {
	char* ident;

	unsigned int arg_count;
	CompArg* args;

	unsigned int sub_decl_count;
	//sub_decls are next in the stack

	unsigned int binding_count;
	Binding* bindings;

} CompDecl;

typedef union InstructionPtr {
	CompDecl* comp_arg;
	Binding* eval;
} InstrPtr;

//instr can be 
typedef struct TopLevelInstruction {
	InstrEnum type;
	InstrPtr instr;
} TopLvlInstrDiscUnion;

typedef union ArgumentIn {
	char* comp_arg_ident;
	Binding* eval;
	bool raw_value;
} UnionArgIn;

typedef struct ArgumentInDiscriminatedUnion {
	ArgInEnum type;
	UnionArgIn data;
} DiscUnionArgIn;

typedef struct InstructionStack {
	Stack* instructions; //TopLvlInstrDiscUnion
	Stack* comp_decls; //CompDecl
	Stack* comp_args; //CompArg
	Stack* bindings; //CompBinding
	Stack* union_arg_in_decls; //UnionArgIn

} InstrStack;

typedef struct ParsedList {
    unsigned int count;
    void* data;
} ParsedList;

InstrStack* InstrStackNew(size_t token_count);
void InstrStackRelease(InstrStack* instr_stack);
typedef int (*InstrFn)(TokenStack* tokens,InstrStack* instructions);

typedef struct ParseClosure{
    InstrFn func;
    InstrStack* t_stack;
} ParseClosure;

PARSE_FUNC_DEF(void,TopLevel);
PARSE_FUNC_DEF(ParsedList, ListCompArg);
PARSE_FUNC_DEF(Binding*,Eval);
PARSE_FUNC_DEF(CompDecl*,CompDecl);
void parseFuncCompArg(TokenStack* token_stack, InstrStack* instr_stack, unsigned int *index,CompArg* out);
#endif
