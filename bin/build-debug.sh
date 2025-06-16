#!/bin/bash
set -e

echo "Building in Debug mode..."
cmake -S . -B bld -DCMAKE_BUILD_TYPE=Debug
cmake --build bld
echo "Debug build complete."
