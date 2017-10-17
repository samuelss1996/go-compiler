#include <ctype.h>
#include "LexicalAnalyzer.h"
#include "../input/InputSystem.h"
#include "../symbols/SymbolsTable.h"


int isMeaninglessSeparator(char character);

int nextToken() {
    int status = 0;
    char readChar;
    char token[200];

    while(1) {
        readChar = nextChar();

        switch(status) {
            case 0:
                if(isalpha(readChar) || readChar == '_') {
                    status = 1;
                } else {
                    return -1;
                }
                break;
            case 1:
                if(isalnum(readChar) || readChar == '_') {
                    status = 1;
                } else {
                    status = 2;
                    moveBack(1);
                }
                break;
            case 2:
                moveBack(1);
                getReadToken(token);

                return findSymbol(token);
        }
    }
}