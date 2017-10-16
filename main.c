#include "input/InputSystem.h"
#include "symbols/SymbolsTable.h"

#include <stdio.h>

// TODO ask the user the file to be compiled
// TODO handle compilation errors using other module
int main() {
    initInputSystem("../concurrentSum.go");
    initSymbolsTable();

    printf("%d\n", findSymbol("break"));

    destroySymbolsTable();
    destroyInputSystem();

    return 0;
}