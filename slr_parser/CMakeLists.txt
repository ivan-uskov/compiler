project (slr_parser)

set(CMAKE_CXX_STANDARD 17)

add_library (${PROJECT_NAME} Generator.h Rules.h src/Generator.cpp Parser.h)

target_link_libraries(${PROJECT_NAME} token)