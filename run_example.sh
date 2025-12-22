#!/bin/bash

mkdir -p ./bin
gcc ./examples/gcy_example.c -o ./bin/gcy_example
./bin/gcy_example
