#include <stdlib.h>
#include "../Definitions.h"
#include "../util/HashTable.h"
#include "../symbols/SymbolsTable.h"
#include "LexicalComponent.h"
#include "../flex/lex.yy.h"


typedef struct {
    FILE* file;
    SymbolsTable symbolsTable;
    HashTable operatorsTable;
} LexicalAnalyzerStruct;

typedef LexicalAnalyzerStruct* LexicalAnalyzer;

/**
 * Crear e incializar el analizador léxico
 * @param lexicalAnalyzer El analizador léxico
 * @param file El nombre del archivo a compilar
 * @param symbolsTable  La tabla de símbolos a usar
 * @param operatorsTable La tabla de operadores a usar
 */
void createLexicalAnalyzer(LexicalAnalyzer *lexicalAnalyzer, char* file, SymbolsTable symbolsTable, HashTable operatorsTable) {
    *lexicalAnalyzer = (LexicalAnalyzer) malloc(sizeof(LexicalAnalyzerStruct));

    (*lexicalAnalyzer)->file = fopen(file, "r");
    (*lexicalAnalyzer)->symbolsTable = symbolsTable;
    (*lexicalAnalyzer)->operatorsTable = operatorsTable;

    yyset_in((*lexicalAnalyzer)->file);
}

/**
 * Obtener el siguiente componente léxico.
 * @param lexicalAnalyzer El analizador léxico
 * @return El siguiente componente léxico.
 */
LexicalComponent nextLexicalComponent(LexicalAnalyzer* lexicalAnalyzer) {
    LexicalComponent result;
    int tokenId = yylex();
    char* tokenStr = yyget_text();

    createLexicalComponent(&result, tokenId, tokenStr);

    return result;
}

/**
 * Destruir el analizador léxico, liberando los recursos
 * @param lexicalAnalyzer
 */
void destroyLexicalAnalyzer(LexicalAnalyzer* lexicalAnalyzer) {
    fclose((*lexicalAnalyzer)->file);
    free(*lexicalAnalyzer);
}