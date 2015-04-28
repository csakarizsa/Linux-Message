#!/bin/bash
gcc main.c -lm -o mainpr
./mainpr $* > log.txt