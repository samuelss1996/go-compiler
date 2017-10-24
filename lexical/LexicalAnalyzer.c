#include <ctype.h>
#include "LexicalAnalyzer.h"
#include "../input/InputSystem.h"
#include "../symbols/SymbolsTable.h"
#include "../Definitions.h"

int nextToken() {
    int status = 0;
    char readChar;
    char token[200];

    while(1) {
        readChar = nextChar();

        switch(status) {
            case 0: // Initial status
                if(isalpha(readChar) || readChar == '_') {
                    status = 1;
                } else if(readChar == '/') {
                    status = 3;
                } else if(readChar == '\'') {
                    status = 7;
                } else if(readChar == '"') {
                    status = 10;
                } else if(readChar == ';') {
                    return PUNCTUATION_SEMICOLON;
                }
                else {
                    return -1;
                }
                break;
            case 1: // Alpha char recognized
                if(isalnum(readChar) || readChar == '_') {
                    status = 1;
                } else {
                    status = 2;
                    moveBack(1);
                }
                break;
            case 2: // Identifier recognized
                moveBack(1);
                getReadToken(token);

                return findSymbol(token);
            case 3: // '/' char recognized
                if(isalpha(readChar) || readChar == '_') {
                    // TODO other things, '/' can be an operator
                } else if(readChar == '/') {
                    status = 4;
                } else if(readChar == '*') {
                    status = 5;
                }
                break;
            case 4: // '//' sequence recognized. Single-line comment. Ends on \n
                if(readChar == '\n') {
                    getReadToken(token);

                    return TOKEN_COMMENT;
                }
                break;
            case 5: // '/*' sequence recognized. Multi-line comment. Ends on '*/'
                if(readChar == '*') {
                    status = 6;
                }
                break;
            case 6: // Recognized '*' on multi-line comment. Possible comment end. Confirmed if '/' found.
                if(readChar == '/') {
                    getReadToken(token);
                    return TOKEN_COMMENT;
                } else {
                    status = 5;
                }
                break;
            case 7: // Recognized "'". Expecting valid rune char or "'" for rune to end
                if(readChar != '\'' && readChar != '\n' && readChar != '\\') {
                    status = 8;
                } else if (readChar == '\\') {
                    status = 9;
                } else if(readChar == '\'') {
                    return TOKEN_RUNE_LITERAL;
                } else {
                    return -1;
                }
                break;
            case 8: // Recognized char inside rune literal. Expecting "'" for rune to end
                if(readChar == '\'') {
                    return TOKEN_RUNE_LITERAL;
                } else {
                    return -1;
                }
            case 9: // Recognized escape char inside rune ('\'). Expecting valid escaped char
                for (int i = 0; i < SIZE_RUNE_VALID_ESCAPED_CHARS; ++i) {
                    if(readChar == ARR_RUNE_VALID_ESCAPED_CHARS[i]) {
                        status = 8;
                        break;
                    }
                }

                if(status == 9) { // Not valid escaped char
                    return -1;
                }
                break;
            case 10: // Recognized '"'. Expecting any valid string char or " for string to end
                if(readChar == '"') {
                    return TOKEN_STRING_LITERAL;
                } else if(readChar == '\\') {
                    status = 11;
                } else if(readChar == '\n') {
                    return -1;
                }
                break;
            case 11: // Recognized escape char inside string ('\'). Expecting valid escaped char
                for (int i = 0; i < SIZE_STRING_VALID_ESCAPED_CHARS; ++i) {
                    if(readChar == ARR_STRING_VALID_ESCAPED_CHARS[i]) {
                        status = 10;
                        break;
                    }
                }

                if(status == 11) { // Not valid escaped char
                    return -1;
                }

                break;
        }
    }
}