#!/bin/bash

g++ -O3 -D_LONG_LONG_LIMB src/*.cpp depends/ffiasm/c/misc.cpp depends/ffiasm/c/naf.cpp depends/ffiasm/c/splitparstr.cpp depends/ffiasm/c/alt_bn128.cpp build/fr.cpp build/fr_raw_arm64.s build/fr_raw_generic.cpp build/fr_generic.cpp build/fq.cpp build/fq_raw_arm64.s build/fq_raw_generic.cpp build/fq_generic.cpp -std=c++17 -I./depends/json/single_include -I./depends/ffiasm/c -I./build/ -I./src -I./package/include -I./depends/gmp/package_macos_arm64/include -L./depends/gmp/package_macos_arm64/lib -lgmp -stdlib=libc++
