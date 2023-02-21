# Задание

+ Написать динамическую библиотеку с функцией `print_hello_from_dyn_runtime_lib()` и
загрузить ее в `hello.c` с помощью `dlopen()`. Объяснить, что происходит:

```bash
user@host:~$ gcc hello_dyn_runtime.c -c -fPIC -o hello_dyn_runtime.o -Wall
user@host:~$ gcc hello_dyn_runtime.o -o libhello_dyn_runtime.so -shared -Wall
user@host:~$ gcc hello.c -o hello -ldl -Wall
```
