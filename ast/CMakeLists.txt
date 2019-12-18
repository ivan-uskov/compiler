project(ast)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        IAST.h
        NumberAST.h src/NumberAST.cpp
        BinaryOperatorAST.h src/BinaryOperatorAST.cpp IASTVisitor.h View.h src/View.cpp ExpressionPairAST.h src/ExpressionPairAST.cpp)