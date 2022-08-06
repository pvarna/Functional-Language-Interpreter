// #include "catch2/catch.hpp"
// #include "lexer.h"
// #include "parser.h"
// #include "interpreter.h"
// #include <iostream>

// TEST_CASE("Evaluation of literals")
// {
//     SECTION("Whole Number")
//     {
//         std::string number = "5";
//         Lexer l(number);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(number, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "5");
//     }

//     SECTION("Fractional Number")
//     {
//         std::string number = "2.8";
//         Lexer l(number);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(number, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "2.8");
//     }

//     SECTION("List")
//     {
//         std::string list = "[ 1 2 3 4 5 ]";
//         Lexer l(list);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(list, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "[1 2 3 4 5]");
//     }
// }

// TEST_CASE("Evaluation of function definition")
// {
//     std::string funcDeclaration = "myList -> [1 2 3 4]";
//     Lexer l(funcDeclaration);

//     std::vector<Token*> tokens = l.tokenize();

//     Parser parser(funcDeclaration, tokens);
//     Interpreter interpreter;

//     REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "0");
// }

// TEST_CASE("Evaliation of some basic functions")
// {
//     SECTION("add")
//     {
//         std::string add = "add(5,6)";
//         Lexer l(add);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(add, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "11");
//     }

//     SECTION("tail")
//     {
//         std::string tail = "tail([1 2 3 4 5])";
//         Lexer l(tail);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(tail, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "[2 3 4 5]");
//     }

//     SECTION("list")
//     {
//         std::string list = "list(1, 2, 3)";
//         Lexer l(list);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(list, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "[1 3 5]");
//     }

//     SECTION("eq")
//     {
//         std::string eq = "eq(5,[5])";
//         Lexer l(eq);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(eq, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "1");
//     }
// }

// TEST_CASE("Evaliation of functions composition")
// {
//     SECTION("Compostiton 1")
//     {
//         std::string comp = "add(mul(5,6),sub(6,2))";
//         Lexer l(comp);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(comp, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "34");
//     }

//     SECTION("Compostiton 2")
//     {
//         std::string comp = "list(head([1 2 3]), add(2,3), head(tail(list(1,2,3))))";
//         Lexer l(comp);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(comp, tokens);
//         Interpreter interpreter;

//         REQUIRE(interpreter.interpret(parser.parse(), std::cout) == "[1 6 11]");
//     }
// }

// TEST_CASE("Use of user defined function")
// {
//     std::string funcDef = "double -> add(#0,#0)";
//     Lexer l1(funcDef);

//     std::vector<Token*> tokens1 = l1.tokenize();

//     Parser parser1(funcDef, tokens1);
//     Interpreter interpreter;

//     REQUIRE(interpreter.interpret(parser1.parse(), std::cout) == "0");

//     std::string func = "double(6)";
//     Lexer l2(func);

//     std::vector<Token*> tokens2 = l2.tokenize();

//     Parser parser2(func, tokens2);

//     REQUIRE(interpreter.interpret(parser2.parse(), std::cout) == "12");
// }

// TEST_CASE("Some runtime errors")
// {
//     SECTION("Division by 0")
//     {
//         std::string f = "div(5,0)";
//         Lexer l(f);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(f, tokens);
//         Interpreter interpreter;

//         REQUIRE_THROWS(interpreter.interpret(parser.parse(), std::cout));
//     }
//     SECTION("Concatenation of infinite lists")
//     {
//         std::string f = "concat(list(1),list(2))";
//         Lexer l(f);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(f, tokens);
//         Interpreter interpreter;

//         REQUIRE_THROWS(interpreter.interpret(parser.parse(), std::cout));
//     }
//     SECTION("Equaling list with more than one element and a number")
//     {
//         std::string f = "eq(2,[2,3])";
//         Lexer l(f);

//         std::vector<Token*> tokens = l.tokenize();

//         Parser parser(f, tokens);
//         Interpreter interpreter;

//         REQUIRE_THROWS(interpreter.interpret(parser.parse(), std::cout));
//     }
// }