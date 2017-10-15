#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

set -e

cd "$DIR"
cd ..
mkdir build
cd build
cmake -Dtravis_build=On ..
make VERBOSE=1
./simple-tool --help
