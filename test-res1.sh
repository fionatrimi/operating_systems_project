#!/bin/bash

clear
echo "___IT'S PIZZA TIME___"
sleep 1
clear

gcc -Wall pizza1.c -pthread
./a.out 8 5
