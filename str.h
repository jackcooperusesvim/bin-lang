#ifndef STR_H
#define STR_H

#include <stdbool.h>
#include <stdlib.h>
#include "stack.h"
bool my_strcmp(char* a, char* b);
unsigned int my_strlen(char* s);
char * my_strcopy(char* s, size_t char_count );
char* my_strcopy_stack(char* in, Stack* stack);
#endif
