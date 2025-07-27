#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "lexer.h"
#include "stack.h"

typedef unsigned short ArgInEnum;
typedef unsigned char InstrEnum;
typedef union ArgumentIn UnionArgIn;
typedef struct ComponentDeclaration CompDecl;

#define Instr_Comp 0
#define Instr_Eval 1
#define Instr_Print 2

#define ArgIn_Eval 0
#define ArgIn_ArgReference 1
#define ArgIn_ArgValue 2

//wire_count of 0 means variadic
typedef struct ComponentArgument {
	char* identifier;
	unsigned int wire_count;
} CompArg;

typedef struct Evaluation {
	char* component_identifier;

	unsigned int arg_count;
	ArgInEnum arg_types;
	UnionArgIn* args;

	struct Evaluation* next;
} Eval;

//is_print discriminates between a print statment (which contains no more data
typedef struct ComponentDeclaration {
	char* component_identifier;

	unsigned int arg_count;
	CompArg* args;

	CompDecl* sub_decls;

	Eval* eval_decls;
} CompDecl;

typedef union InstructionPtr {
	CompDecl* comp_arg;
	Eval* eval;
} InstrPtr;

//instr can be 
typedef struct TopLevelInstruction {
	InstrEnum type;
	InstrPtr instr;
} TopLvlInstrDiscUnion;

//In case I want to store each type separately, I am NOT putting these in a discunion
typedef union ArgumentIn {
	CompArg* comp_arg;
	Eval* eval;
	bool raw_value;
} UnionArgIn;

typedef struct InstructionStack {
	Stack* instructions; //TopLvlInstrDiscUnion
	Stack* comp_decls; //CompDecl
	Stack* comp_args; //CompArg
	Stack* evals; //Eval
	Stack* union_arg_in_decls; //UnionArgIn

} InstrStack;

typedef struct ParsedList {
    unsigned int next_index;
    unsigned int count;
    void* data;
} ParsedList;

typedef struct ParsedSingle{
    unsigned int next_index;
    void* data;
} ParsedSingle;

InstrStack* InstrStackNew(size_t token_count);
void InstrStackRelease(InstrStack* instr_stack);
typedef int (*InstrFn)(TokenStack* tokens,InstrStack* instructions);

typedef struct ParseClosure{
    InstrFn func;
    InstrStack* t_stack;
} ParseClosure;


#endif 
