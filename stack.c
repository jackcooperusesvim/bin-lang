#include "stack.h"

Stack* StackNew(size_t stack_size) {
	Stack* stack_ptr = (Stack*) malloc(sizeof(Stack));
	void * data = malloc(stack_size);

	stack_ptr->size_remaining = stack_size;
	stack_ptr->next = NULL;
	stack_ptr->data = data;
	stack_ptr->stack_ptr = data;
	return stack_ptr;
}
void StackNewHere(size_t stack_size, Stack* stack_ptr) {
	void * data = malloc(stack_size);

	stack_ptr->size_remaining = stack_size;
	stack_ptr->next = NULL;
	stack_ptr->data = data;
	stack_ptr->stack_ptr = data;
}
void StackRelease(Stack* stack) {
	if (stack->next != NULL) {
		StackRelease(stack->next);
	}
	free(stack->data);
	free(stack);
}

void* StackPush(Stack* stack, size_t amount) {
	void* ret_ptr = stack->stack_ptr;
	if (stack->size_remaining > amount) {
		stack->stack_ptr += amount;
		stack->size_remaining -= amount;
	} else {
		if (stack->next == NULL) {
			stack->next = StackNew(amount*50);
		} 
		void* ret_ptr = StackPush(stack->next,amount);
	}
	return ret_ptr;
}

//TODO: IMPLEMENT THIS
void StackPop(size_t stack_size) {
}
