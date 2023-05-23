#!/bin/bash

echo "Compiling..."
g++-12 -std=c++11 main.cpp -o main

echo "Running..."
./main $1