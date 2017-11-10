#!/usr/bin/env bash

set -e

binary_name="$1"
test_file="$2"
cov_dir="$3"

echo "Running with args '$1' '$2' '$3'"

our_test_file=$(basename "$test_file")
test_filename="${our_test_file%.*}"

rm -rf "dir_$test_filename"
mkdir "dir_$test_filename"
cd "dir_$test_filename"
cp "$test_file" .

echo "project(test_$test_filename)"  > CMakeLists.txt
echo "add_library(t OBJECT $our_test_file)" >> CMakeLists.txt
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=On .

LLVM_PROFILE_FILE="$cov_dir/migrate.profraw" valgrind --error-exitcode=1 "$binary_name" -u "$test_file.upgrader" -p . "$our_test_file"
