#!/usr/bin/bash

rm -rf programs
mkdir programs

for program in "create_dir" "list_dir_content" "remove_dir" "create_file" "display_file_content" "remove_file" "create_symbolic_link" "display_symbolic_link" "display_file_by_symbolic_link" "remove_symbolic_link" "create_hard_link" "remove_hard_link" "display_file_permissions" "change_file_permissions"
do
    ln ./task2 programs/$program
done