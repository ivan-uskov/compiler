project(translation)

find_package(LLVM REQUIRED CONFIG)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} Interpreter.h src/Interpreter.cpp LLVMCodeGenerator.h src/LLVMCodeGenerator.cpp begin_llvm.h end_llvm.h FakeLLVMCodeGenerator.h src/FakeLLVMCodeGenerator.cpp)

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

link_directories(${LLVM_LIBDIR})
add_definitions(-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)

llvm_map_components_to_libnames(llvm_libs support core irreader)

target_link_libraries(${PROJECT_NAME}  ${llvm_libs})