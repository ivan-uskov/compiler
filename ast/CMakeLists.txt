project(ast)

set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME}
        IAST.h
        IntAST.h src/IntAST.cpp
        NumberBinaryOperatorAST.h src/NumberBinaryOperatorAST.cpp IASTVisitor.h ExpressionPairAST.h src/ExpressionPairAST.cpp ValueType.h VariableDeclarationAST.h src/VariableDeclarationAST.cpp src/ValueType.cpp View.h src/View.cpp AssignmentAST.h src/AssignmentAST.cpp VariableAccessAST.h src/VariableAccessAST.cpp FunctionCallAST.h src/FunctionCallAST.cpp CompareBinaryOperatorAST.h src/CompareBinaryOperatorAST.cpp IfAST.h src/IfAST.cpp WhileAST.h src/WhileAST.cpp StringAST.h src/StringAST.cpp DoubleAST.h src/DoubleAST.cpp ArrayAssignmentAST.h src/ArrayAssignmentAST.cpp ArrayAccessAST.h src/ArrayAccessAST.cpp BoolAST.h src/BoolAST.cpp)