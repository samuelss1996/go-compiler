#include "input/InputSystem.h"
#include "symbols/SymbolsTable.h"
#include "lexical/LexicalAnalyzer.h"
#include "util/LinkedList.h"
#include "util/HashTable.h"
#include "test/TestRunner.h"

#include <stdio.h>

void fillOperatorsTable(HashTable *operatorsTable) ;

// TODO ask the user the file to be compiled
// TODO handle compilation errors using other module
// TODO handle \nnn and \xnn inside runes and strings
// TODO handle documentation comments
// TODO put keywords in a separated file
// TODO booleans
int main() {
    InputSystem inputSystem;
    SymbolsTable symbolsTable;
    HashTable operatorsTable;
    LexicalAnalyzer lexicalAnalyzer;
    LexicalComponent component;
    int componentId;

    createInputSystem(&inputSystem, "../concurrentSum.go");
    createSymbolsTable(&symbolsTable);
    createHashTable(&operatorsTable);
    createLexicalAnalyzer(&lexicalAnalyzer, inputSystem, symbolsTable, operatorsTable);

    fillOperatorsTable(&operatorsTable);

    do {
        component = nextLexicalComponent(&lexicalAnalyzer);
        printf("%s -> %d\n", getLexicalComponentToken(&component), getLexicalComponentId(&component));

        componentId = getLexicalComponentId(&component);
        destroyLexicalComponent(&component);
    } while(componentId != EOF);

    destroyInputSystem(&inputSystem);
    destroySymbolsTable(&symbolsTable);
    destroyHashTable(&operatorsTable);
    destroyLexicalAnalyzer(&lexicalAnalyzer);

    return 0;
}

void fillOperatorsTable(HashTable *operatorsTable) {
    FILE *operatorsDb = fopen("../db/operators.db", "r");
    char operator[MAXIMUM_OPERATOR_LENGTH + 1];
    int operatorId;

    while(fscanf(operatorsDb, "%s %d", operator, &operatorId) > 0) {
        insertHash(operatorsTable, operator, operatorId);
    }

    fclose(operatorsDb);
}