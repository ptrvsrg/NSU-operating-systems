# Блок задач на “хорошо”

## 6. Разработать собственную функцию для создания ядерных потоков - аналог `pthread_create()`:

```c
    int kernel_thread_create(mythread_t thread, void *(start_routine), void *arg);
```

Функция должна возвращать успех-неуспех.
