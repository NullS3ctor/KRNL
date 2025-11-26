#include <krnl/mem/stack.h>

typedef struct Node {
    uint64_t address;
    int regionID;
    struct Node* next;
} Node;

struct Stack {
    Node* top;
};

Stack* stack_create(void) {
    Stack* stack = malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

void stack_destroy(Stack* stack) {
    while (!stack_is_empty(stack)) {
        stack_pop(stack);
    }
    free(stack);
}

void stack_push(Stack* stack, uint64_t address, int regionID) {
    Node* new_node = malloc(sizeof(Node));
    new_node->address = address;
    new_node->regionID = regionID;
    new_node->next = stack->top;
    stack->top = new_node;
}

uint64_t stack_pop(Stack* stack) {
    Node* top_node = stack->top;
    uint64_t address = top_node->address;
    int regionID = top_node->regionID;
    stack->top = top_node->next;
    free(top_node);
    return address;
}

bool stack_is_empty(Stack* stack) {
    return stack->top == NULL;
}