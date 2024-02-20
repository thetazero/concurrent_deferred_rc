mkdir -p build
cd build
cmake -DCDRC_BENCHMARK=On .. && cmake --build .
