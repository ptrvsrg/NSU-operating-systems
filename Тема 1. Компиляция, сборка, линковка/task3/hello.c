#include <stdio.h>
#include "hello_dynamic.h"

void print_hello(void);

int main(int argc, char **argv) {
    print_hello();
    print_hello_from_dynamic_lib();
    return 0;
}

void print_hello(void) {
    printf("Hello, World!\n");
}
