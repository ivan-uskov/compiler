cmake_minimum_required (VERSION 3.5)

project (compiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")

add_subdirectory(token)
add_subdirectory(lexer)
add_subdirectory(slr_parser)
add_subdirectory(ast)
add_subdirectory(ast_builder)
add_subdirectory(translation)

add_executable(${PROJECT_NAME} compiler.cpp)

target_link_libraries(${PROJECT_NAME} token)
target_link_libraries(${PROJECT_NAME} lexer)
target_link_libraries(${PROJECT_NAME} slr_parser)
target_link_libraries(${PROJECT_NAME} ast)
target_link_libraries(${PROJECT_NAME} ast_builder)
target_link_libraries(${PROJECT_NAME} translation)