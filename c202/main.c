#include <stdio.h>
#include "c202.h"

int main() {
    printf("Hello, World!\n");

	Stack _stack;                                                              \
	Stack *test_stack = &_stack;

    Stack_Init(test_stack);
    for (int i = 0; i < STACK_SIZE; i++)
    {
        Stack_Push(test_stack, 'a' + i % 26);
        printf("Start");
    }

    for (int i = 0; i < _stack.topIndex; ++i) {
        printf("%c ", _stack.array[i]);
    }

    Stack_Dispose(test_stack);

    //test_print_stack(test_stack);
    return 0;
}
