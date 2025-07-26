#include <stdbool.h>
#include "base.h"
#include "stack.h"

struct Variable;

typedef Stack* Runtime ;


typedef WireArr (*Evaluatable) (Stack, WireArr);

// If input_len is -1, the variable is a variadic function
// At some point, I will implement so that if input_len is 0, the variable is a Constant
typedef struct Variable {
	char* name;
	int namelen;
	int output_len;
	int input_len;
	Evaluatable function;
} Variable;

Runtime* RuntimeNew(size_t var_count) {
	return StackNew(var_count*sizeof(Variable));
}

int NewVariable(Runtime* r, char* name, int namelen, int output_len, int input_len, Evaluatable func) {
}


int main(void) {
}
