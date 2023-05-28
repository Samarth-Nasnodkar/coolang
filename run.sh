#!/bin/bash

echo "Compiling..."
g++-12 -std=c++11 main.cpp -o main

if [ $? -eq 0 ]; then
  echo "Running..."
  ./main $1

  if [ $? -eq 0 ]; then
    echo "Cleaning..."
    rm main
  fi
fi