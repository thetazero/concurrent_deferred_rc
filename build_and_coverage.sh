#!/bin/bash

mkdir -p build
cd build
FLAGS="-DCOVERAGE=On -DBUILD_ASAN_TESTS=Off"
cmake -DCDRC_TEST=On $FLAGS .. && cmake --build . && ctest -C Debug --no-tests=error --output-on-failure
