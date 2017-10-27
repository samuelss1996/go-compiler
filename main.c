#include "input/InputSystem.h"
#include "symbols/SymbolsTable.h"
#include "lexical/LexicalAnalyzer.h"
#include "util/LinkedList.h"
#include "util/HashTable.h"
#include "test/TestRunner.h"

#include <stdio.h>

// TODO ask the user the file to be compiled
// TODO handle compilation errors using other module
// TODO handle \nnn and \xnn inside runes and strings
// TODO handle documentation comments
// TODO put keywords in a separated file
int main() {
    initInputSystem("../concurrentSum.go");
    initSymbolsTable();
    initLexicalAnalyzer();

    int token;
    do {
        token = nextToken();
        printf("%d\n", token);
    } while(token != EOF);

    destroyLexicalAnalyzer();
    destroySymbolsTable();
    destroyInputSystem();

    return 0;
}