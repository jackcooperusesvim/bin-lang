#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include "stack.h"
//uses way too much memory
typedef struct DataCable {
	int count;
	bool * values;
} Cable;

typedef Cable (*WireArrFn ) (Stack, Cable) ;

Cable AllocateWireArr(Stack* stack, int wire_count);
Cable Group(Stack* stack,Cable* cables);
Cable Or(Stack* stack,Cable in);
Cable And(Stack* stack,Cable in);
Cable Not(Stack* stack,Cable in);

#endif

