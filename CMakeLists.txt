cmake_minimum_required(VERSION 3.2)
project(ifcount)

option(travis_build "Used for building with travis" OFF)

set (CMAKE_CXX_STANDARD 11)

find_package(LLVM REQUIRED)
find_package(Clang REQUIRED)
link_directories(${LLVM_PATH}/lib)
include_directories(${LLVM_PATH}/include)

find_package(Threads)

option(LLVM_COV "Collect coverage" OFF)
if(LLVM_COV)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
endif()

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS} -fno-rtti -fno-exceptions)

# Now build our tools
add_executable(simple-tool main.cpp)

# Link against LLVM libraries
target_link_libraries(simple-tool
  clang
  clangTooling
  LLVM
  z
  ${CMAKE_THREAD_LIBS_INIT}
  ncurses
)

enable_testing()
file(GLOB test_files "tests/*.cpp")
foreach(test_file ${test_files})
  get_filename_component(test_name "${test_file}" NAME_WE)
  add_test(NAME test_name
           COMMAND "${CMAKE_SOURCE_DIR}/scripts/run_test.sh"
                   "${CMAKE_BINARY_DIR}/simple-tool"
                   "${test_file}"
                   "${CMAKE_BINARY_DIR}"
           WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endforeach()
