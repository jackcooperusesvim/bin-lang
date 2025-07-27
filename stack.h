#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct Stack {
	size_t opt_obj_size;
	size_t size_remaining;
	void* stack_ptr;
	struct Stack* next;
	void* data;
} Stack;

Stack* StackNew(size_t stack_size);
void StackRelease(Stack* stack);
void StackNewHere(size_t stack_size, Stack* stack_ptr) ;

void* StackPush(Stack* stack, size_t amount);
int StackPop(Stack* stack, size_t amount);

void* StackPushRealloc(Stack* stack, size_t amount, size_t realloc_size);

unsigned long StackCountUniform(Stack* stack, bool recursive);
void* StackGetUniform(Stack* stack, size_t obj_size, unsigned int index);


Stack* BlockStackNew(size_t stack_size, size_t obj_size);
void* BlockStackPush(Stack* stack);
void* BlockStackGetIndPtr(Stack* stack, unsigned int index);
int BlockStackPop(Stack* stack);
void* BlockStackGetIndPtr(Stack* stack, unsigned int index);
unsigned int BlockStackSpaceDataAfter(Stack* stack, void* start);
void* StackEndPtr(Stack* stack, void* start);
#endif
