#!/bin/bash

set -e

export CC=/usr/bin/clang
export CXX=/usr/bin/clang++

if [[ $(uname -s) == "Darwin" ]]; then
  ./build_gmp.sh macos_arm64
  mkdir -p build_prover_macos_arm64 
  cd build_prover_macos_arm64
  cmake .. -DTARGET_PLATFORM=macos_arm64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../package
  make -j4 && make install
else
  mkdir -p build_prover
  cd build_prover
  cmake .. -DUSE_ASM=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=../package ..
  make -j$(nproc)
  make install
fi

