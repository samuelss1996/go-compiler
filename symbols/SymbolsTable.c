#include "../Definitions.h"
#include "../util/HashTable.h"
#include "../errors/Errors.h"

#include <stdlib.h>

typedef struct {
    HashTable hashTable;
} SymbolsTableStruct;

typedef SymbolsTableStruct* SymbolsTable;

void fillWithKeywords(SymbolsTable* symbolsTable);

/**
 * Crea e inicializa la tabla de símbolos, rellenándola con las palabras reservadas
 * @param symbolsTable La tabla de śimbolos
 */
void createSymbolsTable(SymbolsTable* symbolsTable) {
    *symbolsTable = (SymbolsTable) malloc(sizeof(SymbolsTableStruct));
    createHashTable(&(*symbolsTable)->hashTable, SYMBOLS_HASH_TABLE_CAPACITY);

    fillWithKeywords(symbolsTable);
}

/**
 * Añade un símbolo a la tabla de símbolos
 * @param symbolsTable La tabla de śimbolos
 * @param token La cadena de texto que representa al símbolo
 * @param id El identificador
 */
void addSymbol(SymbolsTable* symbolsTable, char *token, int id) {
    insertHash(&(*symbolsTable)->hashTable, token, id);
}

/**
 * Consultar un símbolo a la tabla de símbolos
 * @param symbolsTable La tabla de śimbolos
 * @param token La cadena de texto del símbolo a consultar
 * @return El identificador del śimbolo
 */
int findSymbol(SymbolsTable* symbolsTable, char *token) {
    return findHash(&(*symbolsTable)->hashTable, token);
}

// TODO document
int findOrAdd(SymbolsTable* symbolsTable, char* token, int id) {
    int result = findSymbol(symbolsTable, token);

    if(result == TOKEN_NOT_FOUND) {
        result = id;
        addSymbol(symbolsTable, token, result);
    }

    return result;
}

/**
 * Destruye la tabla de símbolos, liberando todos los recursos
 * @param symbolsTable La tabla de śimbolos
 */
void destroySymbolsTable(SymbolsTable* symbolsTable) {
    destroyHashTable(&(*symbolsTable)->hashTable);
    free(*symbolsTable);
}

/**
 * Rellena la tabla de símbolos con las palabras reservadas. Se usa internamente en createSymbolsTable()
 * @param symbolsTable
 */
void fillWithKeywords(SymbolsTable* symbolsTable) {
    FILE *keywordsDb = fopen(KEYWORDS_DB_FILE_PATH, "r");
    char keyword[50];
    int keywordId;

    while(fscanf(keywordsDb, "%s %d", keyword, &keywordId) > 0) {
        addSymbol(symbolsTable, keyword, keywordId);
    }

    fclose(keywordsDb);
}