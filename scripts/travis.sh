#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

set -e

cd "$DIR"
cd ..
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
ctest -j2 --output-on-failure

echo "Running clang-format..."
cd "$DIR"
cd ..

find lib -name "*.cpp" | xargs clang-format -i
find include -name "*.h" | xargs clang-format -i

git diff
git diff-index --quiet HEAD -- || echo "Echo clang-format failed!"
git diff-index --quiet HEAD -- || exit 1
