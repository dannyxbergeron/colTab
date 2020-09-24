#!/bin/bash

name=$1

g++ --std=c++17 "$1".cpp CommandLineOptions.cpp -o colTab -O2 \
        -lboost_program_options -lboost_filesystem -lboost_system
