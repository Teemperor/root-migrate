#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

set -e

cd "$DIR"
cd ..
rm -rf build
mkdir build
cd build
cmake ..
make
ctest -j2
