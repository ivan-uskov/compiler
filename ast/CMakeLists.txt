project (ast)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} Mult.h Node.h Sum.h Visitor.h src/Mult.cpp)