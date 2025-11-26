#ifndef STACK_H
#define STACK_H

#include <krnl/libraries/std/stdint.h>
#include <krnl/libraries/std/stddef.h>
#include <krnl/libraries/std/stdbool.h>
typedef struct Stack Stack;

Stack* stack_create(void);

void stack_destroy(Stack* stack);

void stack_push(Stack* stack, uint64_t address, int regionID);

uint64_t stack_pop(Stack* stack);

bool stack_is_empty(Stack* stack);

#endif