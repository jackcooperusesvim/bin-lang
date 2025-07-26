#ifndef VAR_H
#define VAR_H

#include "base.h"

// If input_len is 0, the variable is an Input
// If input_len is -1, the variable is a variadic function
typedef struct Variable {
	char* name;
	int namelen;
	int input_len;
	WireArrFn function;
} Variable;


#endif
