project(token)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
    src/Token.cpp Token.h
        src/ValueType.cpp Type.h
)