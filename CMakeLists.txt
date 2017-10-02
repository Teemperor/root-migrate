cmake_minimum_required(VERSION 3.2)
project(ifcount)

option(travis_build "Used for building with travis" ON)

set (CMAKE_CXX_STANDARD 11)

if(travis_build)
  include_directories(/usr/lib/llvm-5.0/include/)
  link_directories(/usr/lib/llvm-5.0/lib/)
else()
  find_package(LLVM 5.0 REQUIRED)
  find_package(Clang 5.0 REQUIRED)
endif()

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
