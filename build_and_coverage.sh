#!/bin/bash

mkdir -p build
cd build
cmake -DCDRC_TEST=On -DCOVERAGE=On -DBUILD_ASAN_TESTS=Off .. && cmake --build . && make coverage_report
