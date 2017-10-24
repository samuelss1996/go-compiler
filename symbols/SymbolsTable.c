#include "SymbolsTable.h"
#include "../Definitions.h"

#include <stdio.h>
#include <string.h>

char symbols[300][50];
int ids[300];
int symbolsCount = 0;

// TODO fix this crap
void initSymbolsTable() {
    addSymbol("break", KEYWORD_BREAK);
    addSymbol("default", KEYWORD_DEFAULT);
    addSymbol("func", KEYWORD_FUNC);
    addSymbol("interface", KEYWORD_INTERFACE);
    addSymbol("select", KEYWORD_SELECT);
    addSymbol("case", KEYWORD_CASE);
    addSymbol("defer", KEYWORD_DEFER);
    addSymbol("go", KEYWORD_GO);
    addSymbol("map", KEYWORD_MAP);
    addSymbol("struct", KEYWORD_STRUCT);
    addSymbol("chan", KEYWORD_CHAN);
    addSymbol("else", KEYWORD_ELSE);
    addSymbol("goto", KEYWORD_GOTO);
    addSymbol("package", KEYWORD_PACKAGE);
    addSymbol("switch", KEYWORD_SWITCH);
    addSymbol("const", KEYWORD_CONST);
    addSymbol("fallthrough", KEYWORD_FALLTHROUGH);
    addSymbol("if", KEYWORD_IF);
    addSymbol("range", KEYWORD_RANGE);
    addSymbol("type", KEYWORD_TYPE);
    addSymbol("continue", KEYWORD_CONTINUE);
    addSymbol("for", KEYWORD_FOR);
    addSymbol("import", KEYWORD_IMPORT);
    addSymbol("return", KEYWORD_RETURN);
    addSymbol("var", KEYWORD_VAR);
}

void addSymbol(char *token, int id) {
    strcpy(symbols[symbolsCount], token);
    ids[symbolsCount] = id;

    symbolsCount++;
}

int findSymbol(char *token) {
    for (int i = 0; i < symbolsCount; ++i) {
        if(strcmp(symbols[i], token) == 0) {
            return ids[i];
        }
    }

    return -1;
}

void destroySymbolsTable() {
    // TODO
}
