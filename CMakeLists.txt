cmake_minimum_required(VERSION 3.2)
project(ifcount)

set (CMAKE_CXX_STANDARD 11)

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)

link_directories(${LLVM_PATH}/lib)
include_directories(${LLVM_PATH}/include)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Now build our tools
add_executable(simple-tool main.cpp)

# Link against LLVM libraries
target_link_libraries(simple-tool clangTooling)
