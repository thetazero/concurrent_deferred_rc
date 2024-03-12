mkdir -p build
cd build
cmake -DCDRC_BENCHMARK=On -DCDRC_TEST=Off .. && cmake --build .
