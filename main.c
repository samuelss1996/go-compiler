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

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s fileToBeCompiled.go\n", argv[0]);
        exit(EXIT_CODE_WRONG_ARGUMENTS);
    }

    if(access(KEYWORDS_DB_FILE_PATH, R_OK) != -1) {
        startCompiling(argv[1]);
        return EXIT_CODE_SUCCESS;
    } else {
        unableToOpenDbFile(KEYWORDS_DB_FILE_PATH);
        return EXIT_CODE_UNABLE_TO_OPEN_KEYWORDS_DB;
    }
}

void startCompiling(char* fileToBeCompiled) {
    SymbolsTable symbolsTable;
    LexicalAnalyzer lexicalAnalyzer;
    SyntacticAnalyzer syntacticAnalyzer;

    createSymbolsTable(&symbolsTable);
    createLexicalAnalyzer(&lexicalAnalyzer, fileToBeCompiled, symbolsTable);
    createSyntacticAnalyzer(&syntacticAnalyzer, lexicalAnalyzer);


    startSyntacticAnalyzer(&syntacticAnalyzer);

    destroySymbolsTable(&symbolsTable);
    destroyLexicalAnalyzer(&lexicalAnalyzer);
    destroySyntacticAnalyzer(&syntacticAnalyzer);
}