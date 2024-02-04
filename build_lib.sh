#!/bin/bash

set -e

num_cpus=`grep -c ^processor /proc/cpuinfo`
./build_gmp.sh host || true
mkdir -p build_prover
cd build_prover
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../package ..
make -j$num_cpus 
make install
