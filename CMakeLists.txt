cmake_minimum_required(VERSION 3.2)
project(ifcount)

option(travis_build "Used for building with travis" OFF)

set (CMAKE_CXX_STANDARD 11)

if(travis_build)
  link_directories(/usr/lib/llvm-5.0/lib/)
  set(LLVM_INCLUDE_DIRS /usr/include/llvm-5.0/ /usr/lib/llvm-5.0/include/)
  set(LLVM_DEFINITIONS -DLLVM_BUILD_GLOBAL_ISEL -D_GNU_SOURCE
                       -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS
                       -D__STDC_LIMIT_MACROS)
else()
  find_package(LLVM REQUIRED)
  find_package(Clang REQUIRED)
  link_directories(${LLVM_PATH}/lib)
  include_directories(${LLVM_PATH}/include)
endif()

find_package(Threads)


message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
message(STATUS "LLVM definitions: ${LLVM_DEFINITIONS}")

# Set your project compile flags.
# E.g. if using the C++ header files
# you will need to enable C++11 support
# for your compiler.

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Now build our tools
add_executable(simple-tool main.cpp)

# Link against LLVM libraries
target_link_libraries(simple-tool
  clangAnalysis
  clangARCMigrate
  clangAST
  clangASTMatchers
  clangBasic
  clangCodeGen
  clangDriver
  clangDynamicASTMatchers
  clangEdit
  clangFormat
  clangFrontend
  clangFrontendTool
  clangIndex
  clangLex
  clangParse
  clangRewrite
  clangRewriteFrontend
  clangSema
  clangSerialization
  clangStaticAnalyzerCheckers
  clangStaticAnalyzerCore
  clangStaticAnalyzerFrontend
  clangTooling
  clangToolingCore
  clangToolingRefactor
  LLVMAArch64AsmParser
  LLVMAArch64AsmPrinter
  LLVMAArch64CodeGen
  LLVMAArch64Desc
  LLVMAArch64Disassembler
  LLVMAArch64Info
  LLVMAArch64Utils
  LLVMAMDGPUAsmParser
  LLVMAMDGPUAsmPrinter
  LLVMAMDGPUCodeGen
  LLVMAMDGPUDesc
  LLVMAMDGPUDisassembler
  LLVMAMDGPUInfo
  LLVMAMDGPUUtils
  LLVMAnalysis
  LLVMARMAsmParser
  LLVMARMAsmPrinter
  LLVMARMCodeGen
  LLVMARMDesc
  LLVMARMDisassembler
  LLVMARMInfo
  LLVMAsmParser
  LLVMAsmPrinter
  LLVMBinaryFormat
  LLVMBitReader
  LLVMBitWriter
  LLVMBPFAsmPrinter
  LLVMBPFCodeGen
  LLVMBPFDesc
  LLVMBPFDisassembler
  LLVMBPFInfo
  LLVMCodeGen
  LLVMCore
  LLVMCoroutines
  LLVMCoverage
  LLVMDebugInfoCodeView
  LLVMDebugInfoDWARF
  LLVMDebugInfoMSF
  LLVMDebugInfoPDB
  LLVMDemangle
  LLVMDlltoolDriver
  LLVMExecutionEngine
  LLVMGlobalISel
  LLVMHexagonAsmParser
  LLVMHexagonCodeGen
  LLVMHexagonDesc
  LLVMHexagonDisassembler
  LLVMHexagonInfo
  LLVMInstCombine
  LLVMInstrumentation
  LLVMInterpreter
  LLVMipo
  LLVMIRReader
  LLVMLanaiAsmParser
  LLVMLanaiAsmPrinter
  LLVMLanaiCodeGen
  LLVMLanaiDesc
  LLVMLanaiDisassembler
  LLVMLanaiInfo
  LLVMLibDriver
  LLVMLineEditor
  LLVMLinker
  LLVMLTO
  LLVMMC
  LLVMMCDisassembler
  LLVMMCJIT
  LLVMMCParser
  LLVMMipsAsmParser
  LLVMMipsAsmPrinter
  LLVMMipsCodeGen
  LLVMMipsDesc
  LLVMMipsDisassembler
  LLVMMipsInfo
  LLVMMIRParser
  LLVMMSP430AsmPrinter
  LLVMMSP430CodeGen
  LLVMMSP430Desc
  LLVMMSP430Info
  LLVMNVPTXAsmPrinter
  LLVMNVPTXCodeGen
  LLVMNVPTXDesc
  LLVMNVPTXInfo
  LLVMObjCARCOpts
  LLVMObject
  LLVMObjectYAML
  LLVMOption
  LLVMOrcJIT
  LLVMPasses
  LLVMPowerPCAsmParser
  LLVMPowerPCAsmPrinter
  LLVMPowerPCCodeGen
  LLVMPowerPCDesc
  LLVMPowerPCDisassembler
  LLVMPowerPCInfo
  LLVMProfileData
  LLVMRuntimeDyld
  LLVMScalarOpts
  LLVMSelectionDAG
  LLVMSparcAsmParser
  LLVMSparcAsmPrinter
  LLVMSparcCodeGen
  LLVMSparcDesc
  LLVMSparcDisassembler
  LLVMSparcInfo
  LLVMSupport
  LLVMSymbolize
  LLVMSystemZAsmParser
  LLVMSystemZAsmPrinter
  LLVMSystemZCodeGen
  LLVMSystemZDesc
  LLVMSystemZDisassembler
  LLVMSystemZInfo
  LLVMTableGen
  LLVMTarget
  LLVMTransformUtils
  LLVMVectorize
  LLVMX86AsmParser
  LLVMX86AsmPrinter
  LLVMX86CodeGen
  LLVMX86Desc
  LLVMX86Disassembler
  LLVMX86Info
  LLVMX86Utils
  LLVMXCoreAsmPrinter
  LLVMXCoreCodeGen
  LLVMXCoreDesc
  LLVMXCoreDisassembler
  LLVMXCoreInfo
  LLVMXRay

  z
  ${CMAKE_THREAD_LIBS_INIT}
  ncurses
)
