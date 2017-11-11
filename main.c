#include <stdlib.h>
#include <unistd.h>
#include "symbols/SymbolsTable.h"
#include "lexical/LexicalAnalyzer.h"
#include "util/LinkedList.h"
#include "util/HashTable.h"
#include "test/TestRunner.h"
#include "syntactic/SyntacticAnalyzer.h"
#include "errors/Errors.h"

void startCompiling(char* fileToBeCompiled);
void fillOperatorsTable(HashTable *operatorsTable);

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s fileToBeCompiled.go\n", argv[0]);
        exit(EXIT_CODE_WRONG_ARGUMENTS);
    }

    if(access(KEYWORDS_DB_FILE_PATH, R_OK) != -1) {
        if(access(OPERATORS_DB_FILE_PATH, R_OK) != -1) {
            startCompiling(argv[1]);
            return EXIT_CODE_SUCCESS;
        } else {
            unableToOpenDbFile(OPERATORS_DB_FILE_PATH);
            return EXIT_CODE_UNABLE_TO_OPEN_OPERATORS_DB;
        }
    } else {
        unableToOpenDbFile(KEYWORDS_DB_FILE_PATH);
        return EXIT_CODE_UNABLE_TO_OPEN_KEYWORDS_DB;
    }
}

void startCompiling(char* fileToBeCompiled) {
    SymbolsTable symbolsTable;
    HashTable operatorsTable;
    LexicalAnalyzer lexicalAnalyzer;
    SyntacticAnalyzer syntacticAnalyzer;

    createSymbolsTable(&symbolsTable);
    createHashTable(&operatorsTable, OPERATORS_HASH_TABLE_CAPACITY);
    createLexicalAnalyzer(&lexicalAnalyzer, fileToBeCompiled, symbolsTable, operatorsTable);
    createSyntacticAnalyzer(&syntacticAnalyzer, lexicalAnalyzer);

    fillOperatorsTable(&operatorsTable);

    startSyntacticAnalyzer(&syntacticAnalyzer);

    destroySymbolsTable(&symbolsTable);
    destroyHashTable(&operatorsTable);
    destroyLexicalAnalyzer(&lexicalAnalyzer);
    destroySyntacticAnalyzer(&syntacticAnalyzer);
}

void fillOperatorsTable(HashTable *operatorsTable) {
    FILE *operatorsDb = fopen(OPERATORS_DB_FILE_PATH, "r");
    char operator[MAXIMUM_OPERATOR_LENGTH + 1];
    int operatorId;

    while(fscanf(operatorsDb, "%s %d", operator, &operatorId) > 0) {
        insertHash(operatorsTable, operator, operatorId);
    }

    fclose(operatorsDb);
}