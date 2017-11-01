#include <stdio.h>
#include "../Definitions.h"

void unableToOpenFile(char* filename) {
    printf(ANSI_COLOR_ERROR "Unable to open file '%s'. Check if file exists with proper permissions\n" ANSI_COLOR_RESET, filename);
}

void unableToOpenDbFile(char* filename) {
    printf(ANSI_COLOR_ERROR "Unable to open database file '%s'. This file is required by the compiler. Try to fix this executing 'make' again\n" ANSI_COLOR_RESET, filename);
}

void expectingEndOfString(int line, int column) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Expecting end of string before new line or file end\n" ANSI_COLOR_RESET, line, column);
}

void invalidEscapedCharInsideString(int line, int column, char foundChar) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Invalid character '%c' after backslash inside string\n" ANSI_COLOR_RESET, line, column, foundChar);
}

void invalidSymbol(int line, int column, char foundChar) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Invalid symbol '%c'\n" ANSI_COLOR_RESET, line, column, foundChar);
}