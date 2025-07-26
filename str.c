#include "str.h"
#include "stack.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

unsigned int my_strlen(char* s) {
	unsigned int count = 0;
	for(;s[count]!=*"\0";s++);
	return count;
}

//Optimize
bool my_strcmp(char* a, char* b) {
	unsigned int len_one = my_strlen(a);
	if (len_one == my_strlen(b)) {return false;}
	for (int i = 0; i<len_one; i++) {
		if (b[i] != a[i]) {return false;}
	}
	return true;
}

char * my_strcopy(char* s, size_t char_count ) {
	size_t in_size = (char_count)*sizeof(char);
	char * out = (char*) malloc(in_size+sizeof(char));

	memcpy(out,s,in_size);
	*(out+in_size) = *"\0";
	return out;
}
char * my_strcopy_stack(char* in, Stack* stack) {
	size_t size = my_strlen(in);
	char* out = StackPush(stack, sizeof(char)*(my_strlen(in)+1));
	memcpy(out,in,size);
	*(out+size) = *"\0";
	return out;
}
