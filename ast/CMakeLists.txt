project(ast)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        IAST.h
        NumberAST.h src/NumberAST.cpp
        BinaryOperatorAST.h src/BinaryOperatorAST.cpp IASTVisitor.h ExpressionPairAST.h src/ExpressionPairAST.cpp ValueType.h VariableDeclarationAST.h src/VariableDeclarationAST.cpp src/ValueType.cpp View.h src/View.cpp AssignmentAST.h src/AssignmentAST.cpp VariableAccessAST.h src/VariableAccessAST.cpp FunctionCallAST.h src/FunctionCallAST.cpp)