cmake_minimum_required(VERSION 3.8)
project(GoCompiler)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c input/InputSystem.c input/InputSystem.h symbols/SymbolsTable.c symbols/SymbolsTable.h Definitions.h lexical/LexicalAnalyzer.c lexical/LexicalAnalyzer.h syntactic/SyntacticAnalyzer.c syntactic/SyntacticAnalyzer.h)
add_executable(GoCompiler ${SOURCE_FILES})