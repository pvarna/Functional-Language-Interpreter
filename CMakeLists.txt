cmake_minimum_required(VERSION 3.0.0)
project(Functional-Language-Interpreter VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(ListFunc src/Exceptions/illegalCharacterException.cpp
                     src/Exceptions/missingCharacterException.cpp
                     src/Lexer/lexer.cpp
                     src/Lexer/Tokens/argumentToken.cpp
                     src/Lexer/Tokens/fractionalNumberToken.cpp
                     src/Lexer/Tokens/functionNameToken.cpp
                     src/Lexer/Tokens/token.cpp
                     src/Lexer/Tokens/wholeNumberToken.cpp
                     src/Parser/ASTNode.cpp)
                     
target_include_directories(ListFunc PUBLIC headers/)

add_executable(ConsoleApp main.cpp)
target_link_libraries(ConsoleApp ListFunc)

add_executable(Tests test/lexerTests.cpp)
target_link_libraries(Tests ListFunc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
