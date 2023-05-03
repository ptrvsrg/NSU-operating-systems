#!/usr/bin/bash

rm -rf programs
mkdir programs

for program in "create_dir" "list_dir_content" "remove_dir" "create_file" "display_file_content" "remove_file" "create_sym_link" "display_sym_link_content" "display_file_content_by_sym_link" "remove_sym_link" "create_hard_link" "remove_hard_link" "display_file_permissions" "change_file_permissions"
do
    ln ./task2 programs/$program
done