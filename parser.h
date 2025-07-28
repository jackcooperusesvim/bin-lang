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
#define Instr_Eval 1
#define Instr_Print 2

#define ArgIn_Eval 0
#define ArgIn_ArgReference 1
#define ArgIn_ArgValue 2
 
#define PARSE_FUNC(ret_type, name) ret_type name(TokenStack* token_stack, InstrStack* instr_stack, unsigned int *index)

//wire_count of 0 means variadic
typedef struct ComponentArgument {
	char* ident;
	unsigned int wire_count; //Also range_start
	unsigned int range_end;
	
	//For function arguments
	unsigned int arg_count;
	struct ComponentArgument* args;
} CompArg;

typedef struct EvaluationDecl {
	char* ident;

	unsigned int arg_count;
	DiscUnionArgIn* args;

} EvalDecl;

//is_print discriminates between a print statment (which contains no more data
typedef struct ComponentDeclaration {
	char* ident;

	unsigned int arg_count;
	CompArg* args;

	unsigned int sub_decl_count;
	CompDecl* sub_decls;

	unsigned int eval_count;
	EvalDecl* evals;

} CompDecl;

typedef union InstructionPtr {
	CompDecl* comp_arg;
	EvalDecl* eval;
} InstrPtr;

//instr can be 
typedef struct TopLevelInstruction {
	InstrEnum type;
	InstrPtr instr;
} TopLvlInstrDiscUnion;

typedef union ArgumentIn {
	CompArg* comp_arg;
	EvalDecl* eval;
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
	Stack* evals; //Eval
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

PARSE_FUNC(void,parseTopLevel);
PARSE_FUNC(ParsedList, parseListCompArg);
PARSE_FUNC(CompDecl*,parseCompDecl);
PARSE_FUNC(EvalDecl*,parseEval);
void parseFuncCompArg(TokenStack* token_stack, InstrStack* instr_stack, unsigned int *index,CompArg* out);

#endif 
