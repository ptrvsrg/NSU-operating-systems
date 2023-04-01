#include <dlfcn.h>
#include <stdio.h>

void print_hello(void);

int main(int argc, char **argv) {
    void *hello_dyn_runtime_lib = dlopen("./libhello_dyn_runtime.so", RTLD_LAZY);
    if (!hello_dyn_runtime_lib) {
        fprintf(stderr, "dlopen() error: %s\n", dlerror());
        return 1;
    };

    void (*print_hello_from_dyn_runtime_lib)(void) = dlsym(hello_dyn_runtime_lib, "print_hello_from_dyn_runtime_lib");

    print_hello();
    print_hello_from_dyn_runtime_lib();

    dlclose(hello_dyn_runtime_lib);

    return 0;
}

void print_hello(void) {
    printf("Hello, World!\n");
}
