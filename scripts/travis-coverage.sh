#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

set -e

cd "$DIR"
cd ..
rm -rf build
mkdir build
cd build
cmake -DLLVM_COV=On -DCMAKE_BUILD_TYPE=Debug ..
make
ctest --output-on-failure

binary_name="./root-migrate"

llvm-profdata merge -sparse migrate.profraw -o migrate.profdata
llvm-cov show "$binary_name" -instr-profile=migrate.profdata
llvm-cov report "$binary_name" -instr-profile=migrate.profdata 2>&1 > cov_data
cat cov_data
cov_percent=`cat cov_data | grep "^TOTAL " | awk '{print $13}'`

if [ "$cov_percent" != "100.00%" ]; then
  echo "Coverage decreased $cov_percent"
  exit 1
else
  echo "Coverage is still $cov_percent"
fi
