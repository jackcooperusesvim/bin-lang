#ifndef STACK_H
#define STACK_H

#include <stdlib.h>

typedef struct Stack {
	size_t size_remaining;
	void* stack_ptr;
	struct Stack* next;
	void* data;
} Stack;

Stack* StackNew(size_t stack_size);
void StackRelease(Stack* stack);
void StackNewHere(size_t stack_size, Stack* stack_ptr) ;

void* StackPush(Stack* stack, size_t amount);
void StackPop(size_t stack_size);

Stack StackExpand(Stack* stack, size_t stack_size);

#endif
