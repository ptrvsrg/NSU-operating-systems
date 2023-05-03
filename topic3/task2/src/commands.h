#ifndef COMMANDS_H_
#define COMMANDS_H_

#define SUCCESS 0
#define ERROR (-1)
#define BUFFER_SIZE 256

int create_dir(int argc, char **argv);
int list_dir_content(int argc, char **argv);
int remove_dir(int argc, char **argv);
int create_file(int argc, char **argv);
int display_file_content(int argc, char **argv);
int remove_file(int argc, char **argv);
int create_sym_link(int argc, char **argv);
int display_sym_link_content(int argc, char **argv);
int display_file_content_by_sym_link(int argc, char **argv);
int remove_sym_link(int argc, char **argv);
int create_hard_link(int argc, char **argv);
int remove_hard_link(int argc, char **argv);
int display_file_permissions(int argc, char **argv);
int change_file_permissions(int argc, char **argv);

#endif // COMMANDS_H_
