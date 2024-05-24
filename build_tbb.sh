#!/bin/bash

set -e

cd depends/tbb/oneTBB
# Create binary directory for out-of-source build
mkdir -p build && cd build
# Configure: customize CMAKE_INSTALL_PREFIX and disable TBB_TEST to avoid tests build
cmake -DCMAKE_INSTALL_PREFIX=$(pwd)/installed -DTBB_TEST=OFF ..
# Build
cmake --build .
# Install
cmake --install .

cd ../../..
