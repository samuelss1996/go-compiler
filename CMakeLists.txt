cmake_minimum_required(VERSION 3.8)
project(GoCompiler)

set(CMAKE_C_STANDARD 99)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lfl")
set(SOURCE_FILES main.c input/InputSystem.c input/InputSystem.h symbols/SymbolsTable.c symbols/SymbolsTable.h Definitions.h lexical/LexicalAnalyzer.c lexical/LexicalAnalyzer.h syntactic/SyntacticAnalyzer.c syntactic/SyntacticAnalyzer.h util/HashTable.c util/HashTable.h util/LinkedList.c util/LinkedList.h test/TestHashTable.c test/TestUtils.c test/TestRunner.c test/TestUtils.h test/TestRunner.h test/TestHashTable.h test/TestInputSystem.c test/TestInputSystem.h lexical/LexicalComponent.c lexical/LexicalComponent.h errors/Errors.c errors/Errors.h flex/lex.yy.c flex/lex.yy.h)

add_executable(GoCompiler ${SOURCE_FILES})
target_link_libraries(${CMAKE_PROJECT_NAME} -lfl)