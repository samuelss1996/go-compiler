#include "input/InputSystem.h"
#include "symbols/SymbolsTable.h"
#include "lexical/LexicalAnalyzer.h"
#include "util/LinkedList.h"

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

    LinkedList list = NULL;
    createList(&list);

    append(&list, 2);
    append(&list, 3);
    append(&list, 5);
    append(&list, -1);

    ListNode currentNode = firstNode(&list);

    while(currentNode != NULL) {
        printf("%d\n", nodeValue(&list, currentNode));
        currentNode = nextNode(&list, currentNode);
    }

    destroyList(&list);

    createList(&list);
    
    append(&list, -1);
    append(&list, 3);
    append(&list, 5);
    append(&list, 2);

    currentNode = firstNode(&list);

    while(currentNode != NULL) {
        printf("%d\n", nodeValue(&list, currentNode));
        currentNode = nextNode(&list, currentNode);
    }


    return 0;
}