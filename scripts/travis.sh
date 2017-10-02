#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

cd "$DIR"
cd ..
mkdir build
cd build
cmake ..
make -j2
