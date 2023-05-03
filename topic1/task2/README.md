# Задание

+ Написать статическую библиотеку с функцией `print_hello_from_static_lib()` и использовать ее в `hello.c`:

```bash
user@host:~$ gcc hello_static.c -c -o hello_static.o -Wall 
user@host:~$ ar r libhello_static.a hello_static.o
user@host:~$ gcc hello.c -o hello -lhello_static -L. -Wall
user@host:~$ ./hello
```

+ Посмотреть исполняемый файл на предмет того, будет ли функция `print_hello_from_static_lib()` `unresolved`. Почему? Где находится код этой функции?

```bash
user@host:~$ nm ./hello
```
