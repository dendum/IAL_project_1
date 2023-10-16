#include <stdio.h>
#include "c204.h"

int main() {
    printf("Hello, World!\n");

    char infix[100] = "a+b*(c^d-e)^(f+g*h)-i";

    // Function call
    char* postfix = infix2postfix(infix);
    printf("%s\n", postfix);
    free(postfix);
    return 0;
}
