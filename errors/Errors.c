#include <stdio.h>

void expectingEndOfString(int line, int column) {
    printf("[%d:%d] Expecting end of string before new line\n\t", line, column);
}

void invalidEscapedCharInsideString(int line, int column, char foundChar) {
    printf("[%d:%d] Invalid character '%c' after backslash inside string\n\t", line, column, foundChar);
}

void invalidSymbol(int line, int column, char foundChar) {
    printf("[%d:%d] Invalid symbol '%c'\n\t", line, column, foundChar);
}