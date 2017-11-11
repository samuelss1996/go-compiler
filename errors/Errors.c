#include <stdio.h>
#include "../Definitions.h"

/**
 * Invocado cuando el fichero a compilar no se puede abrir
 * @param filename El nombre del fichero
 */
void unableToOpenFile(char* filename) {
    printf(ANSI_COLOR_ERROR "Unable to open file '%s'. Check if file exists with proper permissions\n" ANSI_COLOR_RESET, filename);
}

/**
 * Invocado cuando archivo *.db (keywords.db / operators.db), necesario para el compilador, no puede ser abierto
 * @param filename El nombre del archivo
 */
void unableToOpenDbFile(char* filename) {
    printf(ANSI_COLOR_ERROR "Unable to open database file '%s'. This file is required by the compiler. Try to fix this executing 'make -B' to rebuild the whole project\n" ANSI_COLOR_RESET, filename);
}

/**
 * Invocado por el analizador léxico cuando una cadena de texto no se cierra antes de un salto de línea o final de fichero
 * @param line El número de línea en la que se detectó el error
 * @param column La columna (número de carácter) en la que se detectó el error
 */
void expectingEndOfString(int line, int column) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Expecting end of string before new line or file end\n" ANSI_COLOR_RESET, line, column);
}

/**
 * Invocado por el analizador léxico cuando en una cadena de texto se detecta un carácter no válido despues de una barra de escape
 * @param line El número de línea en la que se detectó el error
 * @param column La columna (número de carácter) en la que se detectó el error
 * @param foundChar El caracter no válido que se encontró después de la barra de escape
 */
void invalidEscapedCharInsideString(int line, int column, char *string) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Invalid character after backslash inside string: %s\n" ANSI_COLOR_RESET, line, column, string);
}

/**
 * Invocado por el analizador léxico cuando un rune no se cierra antes de un salto de línea o final de fichero
 * @param line El número de línea en la que se detectó el error
 * @param column La columna (número de carácter) en la que se detectó el error
 */
void expectingEndOfRune(int line, int column) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Expecting end of rune before new line or file end\n" ANSI_COLOR_RESET, line, column);
}

/**
 * Invocado por el analizador léxico cuando se detecta un rune vacío ('')
 * @param line El número de línea en la que se detectó el error
 * @param column La columna (número de carácter) en la que se detectó el error
 */
void emptyRune(int line, int column) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Empty rune. Runes must contain exactly one character\n" ANSI_COLOR_RESET, line, column);
}

/**
 * Invocado por el analizador léxico cuando en un rune se detecta un carácter no válido despues de una barra de escape
 * @param line El número de línea en la que se detectó el error
 * @param column La columna (número de carácter) en la que se detectó el error
 * @param foundChar El caracter no válido que se encontró después de la barra de escape
 */
void invalidEscapedCharInsideRune(int line, int column, char *rune) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Invalid character after backslash inside rune: %s\n" ANSI_COLOR_RESET, line, column, rune);
}

/**
 * Invocado por el analizador léxico cuando se detecta un rune con más de un carácter ('ab')
 * @param line El número de línea en la que se detectó el error
 * @param column La columna (número de carácter) en la que se detectó el error
 */
void multiCharacterRune(int line, int column) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Multiple characters found inside rune. Runes must contain exactly one character\n" ANSI_COLOR_RESET, line, column);
}

/**
 * Invocado por el analizador léxico cuando se detecta un símbolo que no se puede interpretar ($, ñ, #, etc.)
 * @param line El número de línea en la que se detectó el error
 * @param column La columna (número de carácter) en la que se detectó el error
 */
void invalidSymbol(int line, int column, char foundChar) {
    printf(ANSI_COLOR_ERROR "[%d:%d] Invalid symbol '%c'\n" ANSI_COLOR_RESET, line, column, foundChar);
}