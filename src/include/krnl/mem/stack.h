#ifndef STACK_H
#define STACK_H

#include <krnl/libraries/std/stdint.h>
#include <krnl/libraries/std/stddef.h>
#include <krnl/libraries/std/stdbool.h>
typedef struct Stack Stack;

// Creates a new stack
Stack* stack_create(void);

// Destroys the stack and frees memory
void stack_destroy(Stack* stack);

// Pushes a new address onto the stack
void stack_push(Stack* stack, uint64_t address, int regionID);

// Pops and returns the top address from the stack
uint64_t stack_pop(Stack* stack);

// Checks if the stack is empty
bool stack_is_empty(Stack* stack);

#endif