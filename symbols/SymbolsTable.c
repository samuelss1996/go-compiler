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

void fillWithKeywords(SymbolsTable* symbolsTable) {
    FILE *keywordsDb = fopen("../db/keywords.db", "r");
    char keyword[50];
    int keywordId;

    while(fscanf(keywordsDb, "%s %d", keyword, &keywordId) > 0) {
        addSymbol(symbolsTable, keyword, keywordId);
    }

    fclose(keywordsDb);
}