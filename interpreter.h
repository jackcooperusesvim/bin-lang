#ifndef INTERP_H
#define INTERP_H

#include "parser.h"
#include "base.h"
#include "stack.h"

typedef struct Component Comp;
typedef union EvaluatableUnion EvalUnion;
typedef struct Evaluation Eval;
typedef struct DataCable Cable;
typedef struct BoundComponent BoundComp;

typedef Cable (*CoreFn) (Stack, Cable) ;


typedef struct Env {
	Stack* commands;
	Stack* tl_components;
	Stack* RawComponent;
	Stack* sub_components;
} TopLvlEnv;


typedef struct Component {
	char* ident;
	unsigned int inputs;
	unsigned int* value_router; //as many as there are "evals" inputs
	Comp* evals;
} Comp;

typedef struct Command {
	Comp component;
	Cable args;
} Cmd;

typedef union Executable {
	RawComponent
} Exe;

//-1 input is variadic
typedef struct RawComponent {
	char* ident;
	unsigned int inputs;

}


Comp* StoreComp(CompDecl decl, Stack* comp_stack) {
	//Gen Openings

	unsigned int inputs = 0
	for (int i=0;i<decl.eval_count;i++) {
		EvalDecl bound_comp = decl.args[i];
	}

	for (int i=0;i<decl.arg_count;i++) {
		CompArg arg = decl.args[i];
	}
}


typedef struct InterpreterEnv {
	Stack* components; //Block-stored "Comp"
} InterpEnv;























//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
// typedef WireArr (*EvalFn) (Stack, WireArr);
//
// typedef union DirectlyEval {
// 	EvalFn func;
// 	WireArr* wire;
// } Deval;
//
// typedef struct BoundComponent{
// 	EvalFn cable;
// 	Comp comp;
// } BoundComp;
//
//
// typedef short EvalEnum;
//
// #define Eval_BoundComp 0;
// #define Eval_SetArg 1;
// #define Eval_Bool 2;
//
//
// typedef union EvaluatableUnion {
// 	BoundComp boundcomp;
// } EvalUnion;
//
//
//
// typedef struct InterpreterEnv {
// 	Stack* components;
// 	Stack* evals;
// 	Stack* contexts;
// 	Stack* root_evals;
// 	Stack* token_data;
// } InterpEnv;
//
// InterpEnv* InterpEvnNew(size_t eval_decl_count,TokenStack token_stack) {
// 	size_t cnt = BlockStackCount(token_stack.data_stack, true);
// 	StackRelease(token_stack.token_stack);
// 	InterpEnv* interp_env = (InterpEnv*) malloc(sizeof(InterpEnv));
//
//
//
// 	interp_env->components = BlockStackNew(cnt,sizeof(EvalFn));
// 	interp_env->token_data = token_stack.data_stack;
// 	interp_env->contexts = BlockStackNew(cnt,sizeof(Comp));
// 	interp_env->root_evals = BlockStackNew(cnt,sizeof(EvalFn));
//
// 	return interp_env;
// }
//
// void InterpEvnRelease(InterpEnv* interp_env) {
// 	StackRelease(interp_env->components);
// 	StackRelease(interp_env->token_data);
// 	StackRelease(interp_env->contexts);
// 	StackRelease(interp_env->root_evals);
// }
//
// //-------------------------------------------------------
// //-------------------------------------------------------
// //-------------------------------------------------------
// //-------------------------------------------------------
// //-------------------------------------------------------
// //-------------------------------------------------------
// //-------------------------------------------------------
// typedef struct Context {
// 	ContCompDecl* owner;
// 	Arg* input_decls;
// 	InputContext* sub_context;
// } InputContext;
//
// typedef struct FunctionArguments{
// } FnArgs;
//
// #endif
