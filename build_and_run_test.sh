#!/bin/bash

mkdir -p build
cd build
FLAGS="-DBUILD_ASAN_TESTS=On "
cmake -DCDRC_TEST=On $FLAGS .. && cmake --build . && ctest -C Debug --no-tests=error --output-on-failure
# cmake -DCDRC_TEST=On -DBUILD_ASAN_TESTS=On .. && cmake --build . && ctest -C Debug --no-tests=error --output-on-failure -R TestWeakPtrLeak
# cmake -DCDRC_TEST=On -DBUILD_ASAN_TESTS=On .. && cmake --build . && ctest -C Debug --no-tests=error --output-on-failure -R TestBug
# cmake -DCDRC_TEST=On -DBUILD_ASAN_TESTS=On .. && cmake --build . && ctest -C Debug --no-tests=error --output-on-failure -R TestSticky
