#!/bin/bash

clear

echo "  ___ _____ _ ____           ";
echo " |_ _|_   _( / ___|          ";
echo "  | |  | | |/\___ \          ";
echo "  | |  | |    ___) |         ";
echo " |____ ___ __________   _    ";
echo " |  _ |_ _|__  |__  /  / \   ";
echo " | |_) | |  / /  / /  / _ \  ";
echo " |  __/| | / /_ / /_ / ___ \ ";
echo " |_____________/________  \_\ ";
echo " |_   _|_ _|  \/  | ____|    ";
echo "   | |  | || |\/| |  _|      ";
echo "   | |  | || |  | | |___     ";
echo "   |_| |___|_|  |_|_____|    ";
echo "                             ";

sleep 2;
clear

gcc -Wall pizza1.c -pthread
./a.out 100 1000
