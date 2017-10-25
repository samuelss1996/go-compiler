#include "input/InputSystem.h"
#include "symbols/SymbolsTable.h"
#include "lexical/LexicalAnalyzer.h"
#include "util/LinkedList.h"
#include "util/HashTable.h"

#include <stdio.h>

// TODO ask the user the file to be compiled
// TODO handle compilation errors using other module
// TODO handle \nnn and \xnn inside runes and strings
// TODO handle documentation comments
// TODO put keywords in a separated file
int main() {
//    initInputSystem("../concurrentSum.go");
//    initSymbolsTable();
//
//    int token;
//    do {
//        token = nextToken();
//        printf("%d\n", token);
//    } while(token != EOF);
//
//    destroySymbolsTable();
//    destroyInputSystem();

    HashTable test;

    createHashTable(&test);
    insertHash(&test, "a", 1);
    insertHash(&test, "b", 2);
    insertHash(&test, "c", 3);
    insertHash(&test, "d", 4);
    insertHash(&test, "e", 5);
    insertHash(&test, "f", 6);

    printf("%d", findHash(&test, "a"));
    printf("%d", findHash(&test, "b"));
    printf("%d", findHash(&test, "c"));
    printf("%d", findHash(&test, "d"));
    printf("%d", findHash(&test, "e"));
    printf("%d", findHash(&test, "f"));

    return 0;
}