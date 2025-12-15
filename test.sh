#!/bin/bash

mkdir -p ./bin
gcc ./tests/test.c -o ./bin/test
./bin/test
