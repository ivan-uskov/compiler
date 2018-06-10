project (lexer)

set(CMAKE_CXX_STANDARD 17)

add_library (${PROJECT_NAME}
    src/Tokenizer.cpp Tokenizer.h
    src/TokenPattern.cpp TokenPattern.h
    src/TokenPatternProvider.cpp TokenPatternProvider.h
    src/TokenRegexPattern.cpp TokenRegexPattern.h
)