#include "stack.h"
#include <stddef.h>
#include <stdbool.h>

Stack* StackNew(size_t stack_size) {
	Stack* stack_ptr = (Stack*) malloc(sizeof(Stack));
	void * data = malloc(stack_size);

	stack_ptr->opt_obj_size = 0;

	stack_ptr->size_remaining = stack_size;
	stack_ptr->next = NULL;
	stack_ptr->data = data;
	stack_ptr->stack_ptr = data;
	return stack_ptr;
}

Stack* BlockStackNew(size_t stack_size, size_t obj_size) {
	Stack* stack_ptr = (Stack*) malloc(sizeof(Stack));
	void * data = malloc(stack_size*obj_size);

	stack_ptr->opt_obj_size= obj_size;
	stack_ptr->size_remaining = stack_size;
	stack_ptr->next = NULL;
	stack_ptr->data = data;
	stack_ptr->stack_ptr = data;
	return stack_ptr;
}

void* BlockStackAdvanceIndPtr(Stack* stack, unsigned int *index) {
	void* data = BlockStackGetIndPtr(stack, *index);
	if (data == stack->stack_ptr) {
		return NULL;
	}
	(*index)++;
	return data;
}
void* BlockStackGetIndPtr(Stack* stack, unsigned int index) {
	unsigned int count = (stack->stack_ptr-stack->data)/stack->opt_obj_size;
	if (count>index) {
		return stack->data+(stack->opt_obj_size*index);
	}
	if (stack->next != NULL) {
		return BlockStackGetIndPtr(stack->next, index-count);
	} else {
		return NULL;
	}
}

//BUG: DOES THIS EVEN WORK????
void StackNewHere(size_t stack_size, Stack* stack_ptr) {
	void * data = malloc(stack_size);

	stack_ptr->size_remaining = stack_size;
	stack_ptr->next = NULL;
	stack_ptr->data = data;
	stack_ptr->stack_ptr = data;
}

unsigned long BlockStackCount(Stack* stack, bool recursive) {
	ptrdiff_t size = stack->stack_ptr-(stack->data);
	unsigned long size_rest = 0;
	if (stack->next != NULL && recursive) {
		size_rest = BlockStackCount(stack->next,recursive);
	}
	return (unsigned long) (size/stack->opt_obj_size)+size_rest;
}


// __attribute__((deprecated))
// void* BlockStackGetUniformRec(Stack* stack, size_t obj_size, unsigned int index) {
// 	unsigned long count_this_stack = BlockStackCount(stack, obj_size);
// 	if (index>=count_this_stack) {
// 		return BlockStackGetUniformRec(stack->next,obj_size, index-count_this_stack);
// 	}
// 	return stack->data+(obj_size*index);
// }
//
// __attribute__((deprecated))
// void* BlockStackGetUniform(Stack* stack, size_t obj_size, unsigned int index) {
// 	unsigned long total_count = BlockStackCount(stack, obj_size);
// 	if (index>=total_count) {return NULL;}
// 	return BlockStackGetUniformRec(stack->next,obj_size, index);
// }

void StackRelease(Stack* stack) {
	if (stack->next != NULL) {
		StackRelease(stack->next);
	}
	free(stack->data);
	free(stack);
}

void* StackPushRaw(Stack* stack, size_t amount, size_t realloc_size, bool stay_local) {
	void* ret_ptr = stack->stack_ptr;
	if (stack->size_remaining > amount) {
		stack->stack_ptr += amount;
		stack->size_remaining -= amount;
	} else {
		if (stay_local) {return NULL;}
		if (stack->next == NULL) {
			stack->next = StackNew(realloc_size);
			stack->next->opt_obj_size = stack->opt_obj_size;
		} 
		void* ret_ptr = StackPush(stack->next,amount);
	}
	return ret_ptr;
}

void* StackEndPtr(Stack* stack, void* start) {
	return stack->stack_ptr+stack->size_remaining;
}
void* StackPush(Stack* stack, size_t amount) {
	return StackPushRaw(stack,amount,amount*10000,false);
}

void* BlockStackPush(Stack* stack) {
	return StackPushRaw(stack,stack->opt_obj_size,stack->opt_obj_size*10000,false);
}

int BlockStackPop(Stack* stack) {
	return StackPop(stack,stack->opt_obj_size);
}


int StackPopNotRec(Stack* stack, size_t amount) {
	if (amount >= (stack->stack_ptr-stack->data)) {
		stack->stack_ptr = stack->data;
		return 1;
	} else {
		stack->stack_ptr -= amount;
		return 0;
	}
}

int StackPop(Stack* stack, size_t amount) {
	if (stack->next == NULL) {
		StackPopNotRec(stack, amount);
		return 0;
	} else {
		if (stack->next->data > stack->next->stack_ptr) {
			StackPopNotRec(stack,amount);
			return 1;
		} else {
			return StackPop(stack->data,amount);
		}
	}
}

int stack_test(void) {
	Stack* stack_ptr = StackNew(100);
	int* intarr = StackPush(stack_ptr, sizeof(int)*4);
	intarr[0] = 5;
	int* intarr2 = StackPush(stack_ptr, sizeof(int)*4);
	intarr2[0] = 6;
	intarr[0] = 1;
	return 1;
}
