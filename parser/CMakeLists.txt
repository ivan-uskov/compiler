project (parser)

set(CMAKE_CXX_STANDARD 17)

add_library (${PROJECT_NAME} Generator.h Rules.h src/Generator.cpp Parser.h)