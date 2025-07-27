#include "str.h"
#include "stack.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int my_strlen(char* s) {
	unsigned int count = 0;
	for(;s[count]!='\0';count++);
	return count;
}

//Optimize
bool my_strcmp(char* a, char* b) {
	unsigned int len_one = my_strlen(a);
	if (len_one != my_strlen(b)) {return false;}
	for (int i = 0; i<len_one; i++) {
		if (b[i] != a[i]) {return false;}
	}
	return true;
}

char * my_strcopy(char* s, size_t char_count ) {
	char * out = (char*) malloc(char_count+1);
	if (out == NULL) {perror("failed to allocate for my_strcopy");exit(1);}

	memcpy(out,s,char_count+1);
	*(out+char_count) = '\0';
	return out;
}
char * my_strcopy_stack(char* in, Stack* stack) {
	size_t size = my_strlen(in);
	char* out = StackPush(stack, size+1);
	memcpy(out,in,size+1);
	*(out+size) = '\0';
	return out;
}
