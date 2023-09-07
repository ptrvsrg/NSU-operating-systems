В данном репозитории лежат лабораторные работы по дисциплине "Операционные системы" 4-5 семестра ФИТ НГУ.

+ [Компиляция, сборка, линковка](./compiling-building-running/)

  + [Исполняемый файл](./compiling-building-running/executable/)
  + [Статическая библиотека](./compiling-building-running/static-library/)
  + [Динамическая библиотека (загрузка при создании процесса)](./compiling-building-running/dynamic-library/)
  + [Динамическая библиотека (загрузка при выполнении программы)](./compiling-building-running/dynamic-library-runtime/)

+ [Системные вызовы](./system-calls/)

  + [Системный вызов `write` и обёртка над ним, статистика системных вызовов](./system-calls/write-syscall/)
  + [Инструкция `syscall`](./system-calls/syscall-instruction/)
  + [Печать системных вызовов](./system-calls/strace/)

+ [Файлы и файловые системы](./files-and-file-systems/)

  + [Копирование задом наперёд](./files-and-file-systems/reverse-directory/)
  + [Создание, чтение, изменение прав доступа и удаление файлов, каталогов, символьных и жестких ссылок](./files-and-file-systems/commands-for-files/)
  + [Печать `/proc/<pid>/pagemap`](./files-and-file-systems/print-pagemap/)

+ [Адресное пространство процесса](./process-address-space/)

  + [Структура адресного пространства](./process-address-space/address-space-structure/)
  + [Управление адресным пространством](./process-address-space/adress-space-management/)
  + [Самодельная куча](./process-address-space/heap/)

+ [Создание, завершение процесса](./process-creation-and-termination/)

  + [Жизненный цикл процесса](./process-creation-and-termination/process-life-cycle/)
  + [Процесс в состоянии зомби](./process-creation-and-termination/zombie-process/)
  + [Создание процесса при помощи системного вызова `clone(2)`](./process-creation-and-termination/clone-system-call/)

+ [Межпроцессное взаимодействие](./inter-process-communication/)

  + [Взаимодействие процессов через разделяемую память](./inter-process-communication/shared-memory/)
  + [Взаимодействие процессов через `pipe`](./inter-process-communication/pipe/)
  + [Взаимодействие через доменные сокеты](./inter-process-communication/domain-socket/)

+ [Сеть](./network/)

  + [UDP - эхо сервер](./network/udp-echo-server/)
  + [TCP - эхо сервер](./network/tcp-echo-server-1/)
  + [ТСP - эхо сервер №2](./network/tcp-echo-server-2/)

+ [Понятие пользователя. Управление правами](./users-and-rights-management/)

  + [Использование SUID-бита для доступа к файлам](./users-and-rights-management/%20suid-bit-for-file-access/)
