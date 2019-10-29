project(ast_builder)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        ASTBuilder.h
        src/ASTBuilder.cpp
        )

target_link_libraries(${PROJECT_NAME} token ast)