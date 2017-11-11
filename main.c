#include <stdlib.h>
#include <unistd.h>
#include "symbols/SymbolsTable.h"
#include "lexical/LexicalAnalyzer.h"
#include "util/LinkedList.h"
#include "util/HashTable.h"
#include "test/TestRunner.h"
#include "syntactic/SyntacticAnalyzer.h"
#include "errors/Errors.h"

int startCompiling(char* fileToBeCompiled);

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: %s fileToBeCompiled.go\n", argv[0]);
        exit(EXIT_CODE_WRONG_ARGUMENTS);
    }

    if(access(KEYWORDS_DB_FILE_PATH, R_OK) != -1) {
        return startCompiling(argv[1]);
    } else {
        unableToOpenDbFile(KEYWORDS_DB_FILE_PATH);
        return EXIT_CODE_UNABLE_TO_OPEN_KEYWORDS_DB;
    }
}

int startCompiling(char* fileToBeCompiled) {
    SymbolsTable symbolsTable;
    LexicalAnalyzer lexicalAnalyzer;
    SyntacticAnalyzer syntacticAnalyzer;

    FILE* file = fopen(fileToBeCompiled, "r");

    if (file != NULL) {
        createSymbolsTable(&symbolsTable);
        createLexicalAnalyzer(&lexicalAnalyzer, file, symbolsTable);
        createSyntacticAnalyzer(&syntacticAnalyzer, lexicalAnalyzer);

        startSyntacticAnalyzer(&syntacticAnalyzer);

        destroySymbolsTable(&symbolsTable);
        destroyLexicalAnalyzer(&lexicalAnalyzer);
        destroySyntacticAnalyzer(&syntacticAnalyzer);

        fclose(file);
        return EXIT_CODE_SUCCESS;
    } else {
        unableToOpenFile(fileToBeCompiled);
        return EXIT_CODE_UNABLE_TO_OPEN_FILE;
    }
}