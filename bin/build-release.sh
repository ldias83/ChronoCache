#!/bin/bash
set -e
echo "Building in Release mode with -O3 optimizations and debug symbols..."

cmake -S . -B bld -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_CXX_FLAGS_RELEASE="-O3 -march=native -g \
      -fno-omit-frame-pointer \
      -fno-inline-small-functions \
      -fno-inline-functions-called-once"

cmake --build bld
echo "Release build complete."

