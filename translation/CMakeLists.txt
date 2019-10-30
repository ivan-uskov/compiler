project(translation)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} Interpreter.h src/Interpreter.cpp)