#ifndef BASE_H
#define BASE_H

#include <stdbool.h>
#include "stack.h"
typedef struct WireArr {
	int count;
	bool * values;
} WireArr;

typedef WireArr (*WireArrFn ) (Stack, WireArr) ;

WireArr AllocateWireArr(Stack* stack, int wire_count);
WireArr Group(Stack* stack,WireArr in);
WireArr Or(Stack* stack,WireArr in);
WireArr And(Stack* stack,WireArr in);
WireArr Not(Stack* stack,WireArr in);

#endif

