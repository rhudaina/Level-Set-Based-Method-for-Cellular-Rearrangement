#!/bin/bash
clear
rm ./cell.out
rm -r ./output
mkdir ./output

echo -e "Running proposed level set-based scheme..."
gcc -Wall functions.c main.c -g -lfftw3 -lm -o cell.out
./cell.out ../data/parameters.txt ../data/initconfig.txt

echo -e "Done!\n"
