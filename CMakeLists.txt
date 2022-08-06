cmake_minimum_required(VERSION 3.0.0)
project(Functional-Language-Interpreter VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(ListFunc src/Utils/stringConverter.cpp
                     src/Exceptions/illegalCharacterException.cpp
                     src/Exceptions/missingCharacterException.cpp
                     src/Exceptions/illegalSyntaxException.cpp
                     src/Lexer/lexer.cpp
                     src/Lexer/Tokens/argumentToken.cpp
                     src/Lexer/Tokens/fractionalNumberToken.cpp
                     src/Lexer/Tokens/functionNameToken.cpp
                     src/Lexer/Tokens/token.cpp
                     src/Lexer/Tokens/wholeNumberToken.cpp
                     src/Parser/Nodes/ASTNode.cpp
                     src/Parser/Nodes/ASTUnaryFunctionNode.cpp
                     src/Parser/Nodes/ASTBinaryFunctionNode.cpp
                     src/Parser/Nodes/ASTTernaryFunctionNode.cpp
                     src/Parser/Nodes/ASTN_aryFunctionNode.cpp
                     src/Parser/Nodes/ASTUserFunctionNode.cpp
                     src/Parser/Nodes/ASTListNode.cpp
                     src/Parser/parser.cpp
                     src/Interpreter/Literals/literal.cpp
                     src/Interpreter/Literals/wholeNumberLiteral.cpp
                     src/Interpreter/Literals/fractionalNumberLiteral.cpp
                     src/Interpreter/Literals/userDefinedListLiteral.cpp
                     src/Interpreter/Literals/functionDefinedListLiteral.cpp
                     src/Interpreter/interpreter.cpp
                     src/ListFunc/listFunc.cpp)
                     
target_include_directories(ListFunc PUBLIC headers/Exceptions
                                           headers/Lexer
                                           headers/Lexer/Tokens
                                           headers/Parser
                                           headers/Parser/Nodes
                                           headers/Interpreter
                                           headers/Interpreter/Literals
                                           headers/ListFunc
                                           headers/Utils)

add_executable(ConsoleApp main.cpp)
target_link_libraries(ConsoleApp ListFunc)

add_executable(Tests test/lexerTests.cpp test/parserTests.cpp test/interpreterTests.cpp)
# add_executable(Tests test/parserTests.cpp)
target_link_libraries(Tests ListFunc)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
