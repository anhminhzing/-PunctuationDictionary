#!/bin/zsh

echo "Compiling project..."

clang++ main.cpp -std=c++17 -o punctuation_project

if [ $? -ne 0 ]; then
    echo "Compile failed."
    exit 1
fi

echo "Running project..."

./punctuation_project

echo "Done."
