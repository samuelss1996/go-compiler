#include <stdlib.h>
#include "input/InputSystem.h"
#include "symbols/SymbolsTable.h"
#include "lexical/LexicalAnalyzer.h"
#include "util/LinkedList.h"
#include "util/HashTable.h"
#include "test/TestRunner.h"
#include "syntactic/SyntacticAnalyzer.h"
#include "errors/Errors.h"

void startCompiling(char* fileToBeCompiled);
void fillOperatorsTable(HashTable *operatorsTable);

// TODO check memory leaks
int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s fileToBeCompiled.go\n", argv[0]);
        exit(EXIT_CODE_WRONG_ARGUMENTS);
    }

    startCompiling(argv[1]);

    return EXIT_CODE_SUCCESS;
}

void startCompiling(char* fileToBeCompiled) {
    InputSystem inputSystem;
    SymbolsTable symbolsTable;
    HashTable operatorsTable;
    LexicalAnalyzer lexicalAnalyzer;
    SyntacticAnalyzer syntacticAnalyzer;

    createInputSystem(&inputSystem, fileToBeCompiled);
    createSymbolsTable(&symbolsTable);
    createHashTable(&operatorsTable);
    createLexicalAnalyzer(&lexicalAnalyzer, inputSystem, symbolsTable, operatorsTable);
    createSyntacticAnalyzer(&syntacticAnalyzer, lexicalAnalyzer);

    fillOperatorsTable(&operatorsTable);

    startSyntacticAnalyzer(&syntacticAnalyzer);

    destroyInputSystem(&inputSystem);
    destroySymbolsTable(&symbolsTable);
    destroyHashTable(&operatorsTable);
    destroyLexicalAnalyzer(&lexicalAnalyzer);
    destroySyntacticAnalyzer(&syntacticAnalyzer);
}

void fillOperatorsTable(HashTable *operatorsTable) {
    FILE *operatorsDb = fopen(OPERATORS_DB_FILE_PATH, "r");
    char operator[MAXIMUM_OPERATOR_LENGTH + 1];
    int operatorId;

    if(operatorsDb == NULL) {
        unableToOpenDbFile(OPERATORS_DB_FILE_PATH);
        exit(EXIT_CODE_UNABLE_TO_OPEN_OPERATORS_DB);
    }

    while(fscanf(operatorsDb, "%s %d", operator, &operatorId) > 0) {
        insertHash(operatorsTable, operator, operatorId);
    }

    fclose(operatorsDb);
}