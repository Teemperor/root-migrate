#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

set -e

cd "$DIR"
cd ..
rm -rf build
mkdir build
cd build
cmake -DLLVM_COV=On ..
make
ctest -j2

llvm-profdata merge -sparse migrate.profraw -o migrate.profdata
llvm-cov show ./simple-tool -instr-profile=migrate.profdata
cov_percent=`llvm-cov report ./simple-tool -instr-profile=migrate.profdata | grep "^TOTAL " | awk '{print $13}'`

if [ "$cov_percent" != "100.00%" ]; then
  echo "Coverage decreased $cov_percent"
  exit 1
else
  echo "Coverage is still $cov_percent"
fi
