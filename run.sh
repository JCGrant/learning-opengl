#!/bin/bash

set -e
mkdir -p build
cd build
cmake ..
make -j4
cp compile_commands.json ..
cd learnOpenGL
./learnOpenGL
