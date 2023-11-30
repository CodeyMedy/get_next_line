#!/bin/bash

clang -fsanitize=address -g main.c get_next_line.c get_next_line_utils.c -o program
echo "Compiled with AddressSanitizer using Clang"

./program 2>&1
