#include "../Definitions.h"
#include "../util/HashTable.h"

#include <stdlib.h>

typedef struct {
    HashTable hashTable;
} SymbolsTableStruct;

typedef SymbolsTableStruct* SymbolsTable;

void fillWithKeywords(SymbolsTable* symbolsTable);

void createSymbolsTable(SymbolsTable* symbolsTable) {
    *symbolsTable = (SymbolsTable) malloc(sizeof(SymbolsTableStruct));
    createHashTable(&(*symbolsTable)->hashTable);

    fillWithKeywords(symbolsTable);
}

void addSymbol(SymbolsTable* symbolsTable, char *token, int id) {
    insertHash(&(*symbolsTable)->hashTable, token, id);
}

int findSymbol(SymbolsTable* symbolsTable, char *token) {
    return findHash(&(*symbolsTable)->hashTable, token);
}

void destroySymbolsTable(SymbolsTable* symbolsTable) {
    destroyHashTable(&(*symbolsTable)->hashTable);
    free(*symbolsTable);
}

// TODO fix this crap
void fillWithKeywords(SymbolsTable* symbolsTable) {
    addSymbol(symbolsTable, "break", KEYWORD_BREAK);
    addSymbol(symbolsTable, "default", KEYWORD_DEFAULT);
    addSymbol(symbolsTable, "func", KEYWORD_FUNC);
    addSymbol(symbolsTable, "interface", KEYWORD_INTERFACE);
    addSymbol(symbolsTable, "select", KEYWORD_SELECT);
    addSymbol(symbolsTable, "case", KEYWORD_CASE);
    addSymbol(symbolsTable, "defer", KEYWORD_DEFER);
    addSymbol(symbolsTable, "go", KEYWORD_GO);
    addSymbol(symbolsTable, "map", KEYWORD_MAP);
    addSymbol(symbolsTable, "struct", KEYWORD_STRUCT);
    addSymbol(symbolsTable, "chan", KEYWORD_CHAN);
    addSymbol(symbolsTable, "else", KEYWORD_ELSE);
    addSymbol(symbolsTable, "goto", KEYWORD_GOTO);
    addSymbol(symbolsTable, "package", KEYWORD_PACKAGE);
    addSymbol(symbolsTable, "switch", KEYWORD_SWITCH);
    addSymbol(symbolsTable, "const", KEYWORD_CONST);
    addSymbol(symbolsTable, "fallthrough", KEYWORD_FALLTHROUGH);
    addSymbol(symbolsTable, "if", KEYWORD_IF);
    addSymbol(symbolsTable, "range", KEYWORD_RANGE);
    addSymbol(symbolsTable, "type", KEYWORD_TYPE);
    addSymbol(symbolsTable, "continue", KEYWORD_CONTINUE);
    addSymbol(symbolsTable, "for", KEYWORD_FOR);
    addSymbol(symbolsTable, "import", KEYWORD_IMPORT);
    addSymbol(symbolsTable, "return", KEYWORD_RETURN);
    addSymbol(symbolsTable, "var", KEYWORD_VAR);
}