#include <stdbool.h>
#include "base.h"
#include "stack.h"

struct InstructionDU;

typedef struct EvaluationCall EvalCall;
typedef struct ComponentEvaluation CompEval;
typedef struct ComponentDecl CompDecl;
typedef struct ComponentArgument Arg;
typedef struct Context InputContext;
typedef struct ContextComponentDecl ContCompDecl;

typedef unsigned short InstructionType;
typedef unsigned char IdentifierType;
typedef short EvalType;

#define EnumComponentInstruction (InstructionType) 0
#define EnumContextComponentInstruction (InstructionType) 1
#define EnumExecuteInstruction (InstructionType) 2

typedef WireArr (*Evaluatable) (Stack, WireArr);

typedef struct Context {
	ContCompDecl* owner;
	Arg* input_decls;
	InputContext* sub_context;
} InputContext;

typedef struct Component {
	char* identifier;
	unsigned int wire_count;
	InputContext* input_context;
	struct Component* output_decls;
	EvalCall* sub_eval_types;
	void** sub_evals;
} Comp;

typedef char UseInputAs;
#define UseInputAs_Declaration 'a';
#define UseInputAs_Argument 'b';

typedef char ArgType;
#define ArgType_Eval 'a';
#define ArgType_ArgReference 'j';
#define ArgType_ArgDeclaration 'g';

// a wire_count of 0 indicates a variadic
typedef struct ComponentArgument {
	char* identifier;
	unsigned int wire_count;
} Arg;


typedef struct ComponentDecl {
	char* identifier;

	unsigned int arg_decl_count;
	Arg* arg_decls;

	CompDecl* sub_declarations;
	unsigned int sub_decl_count;

	unsigned int eval_count;
	CompEval* eval_declarations;
} CompDecl;

typedef struct ComponentEvaluation {
	char* identifier;

	unsigned int arg_bind_count;
	Arg* arg_bindings;
} CompEval;


// If input_len is -1, the variable is a variadic function
// At some point, I will implement so that if input_len is 0, the variable is a Constant
typedef struct InstructionDU {
	InstructionType instruction;
	void* data;
	char* name;
	int namelen;
	int output_len;
	int input_len;
	Evaluatable function;
} InstructionDU;

Interpreter* InterpreterNew(size_t var_count) {
	return StackNew(var_count*sizeof(Variable));
}

int NewVariable(Interpreter* t, char* name, int namelen, int output_len, int input_len, Evaluatable func) {
}


int main(void) {
}
