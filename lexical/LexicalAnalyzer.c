#include <ctype.h>
#include <stdlib.h>
#include "../Definitions.h"
#include "../util/HashTable.h"
#include "../input/InputSystem.h"
#include "../symbols/SymbolsTable.h"
#include "LexicalComponent.h"

#define SIZE_RUNE_VALID_ESCAPED_CHARS 9
#define SIZE_STRING_VALID_ESCAPED_CHARS 9

char ARR_RUNE_VALID_ESCAPED_CHARS[] = {'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '\''};
char ARR_STRING_VALID_ESCAPED_CHARS[] = {'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '"'};

typedef struct {
    InputSystem inputSystem;
    SymbolsTable symbolsTable;
    HashTable operatorsTable;
} LexicalAnalyzerStruct;

typedef LexicalAnalyzerStruct* LexicalAnalyzer;

int mainAutomaton(LexicalAnalyzer* lexicalAnalyzer);
int recognizeOperator(LexicalAnalyzer* lexicalAnalyzer);
short isInteger(LexicalAnalyzer* lexicalAnalyzer);
short isFloat(LexicalAnalyzer* lexicalAnalyzer);
short isImaginary(LexicalAnalyzer* lexicalAnalyzer);
short isOctal(char digit);

void createLexicalAnalyzer(LexicalAnalyzer *lexicalAnalyzer, InputSystem inputSystem, SymbolsTable symbolsTable, HashTable operatorsTable) {
    *lexicalAnalyzer = (LexicalAnalyzer) malloc(sizeof(LexicalAnalyzerStruct));

    (*lexicalAnalyzer)->inputSystem = inputSystem;
    (*lexicalAnalyzer)->symbolsTable = symbolsTable;
    (*lexicalAnalyzer)->operatorsTable = operatorsTable;
}

LexicalComponent nextLexicalComponent(LexicalAnalyzer* lexicalAnalyzer) {
    int componentId = mainAutomaton(lexicalAnalyzer);
    char componentToken[200]; // TODO
    LexicalComponent result;

    getReadToken(&(*lexicalAnalyzer)->inputSystem, componentToken);
    confirmToken(&(*lexicalAnalyzer)->inputSystem);

    if(componentId == TOKEN_COMMENT || componentId == TOKEN_BLANK) {
        return nextLexicalComponent(lexicalAnalyzer);
    }

    createLexicalComponent(&result, componentId, componentToken);

    return result;
}

void destroyLexicalAnalyzer(LexicalAnalyzer* lexicalAnalyzer) {
    free(*lexicalAnalyzer);
}

int alphanumericAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    char readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);
    char token[200]; // TODO
    int tokenId;

    while(isalnum(readChar) || readChar == '_') {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);
    }

    moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
    getReadToken(&(*lexicalAnalyzer)->inputSystem, token);

    tokenId = findSymbol(&(*lexicalAnalyzer)->symbolsTable, token);

    if(tokenId == TOKEN_NOT_FOUND) {
        addSymbol(&(*lexicalAnalyzer)->symbolsTable, token, TOKEN_IDENTIFIER);
        return TOKEN_IDENTIFIER;
    }
}

int commentsAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0:
                switch(readChar) {
                    case '/': status = 1; break;
                    case '*': status = 2; break;
                    default:
                        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                        return recognizeOperator(lexicalAnalyzer);
                }
                break;
            case 1:
                if(readChar == '\n') {
                    moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                    return TOKEN_COMMENT;
                }
                break;
            case 2:
                switch(readChar) {
                    case '*': status = 3; break;
                    case EOF: return ERROR_CODE;
                }
                break;
            case 3:
                switch(readChar) {
                    case '/': return TOKEN_COMMENT;
                    case EOF: return ERROR_CODE;
                    default: status = 2; break;
                }
                break;
        }
    }
}

int runesAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0:
                switch(readChar) {
                    case '\\': status = 1; break;
                    case '\n': case '\'': return ERROR_CODE;
                    default: status = 2; break;
                }
                break;
            case 1:
                for (int i = 0; i < SIZE_RUNE_VALID_ESCAPED_CHARS; ++i) {
                    if(readChar == ARR_RUNE_VALID_ESCAPED_CHARS[i]) {
                        status = 2;
                        break;
                    }
                }

                if(status == 1) return ERROR_CODE;
                break;
            case 2:
                switch(readChar) {
                    case '\'': return TOKEN_RUNE_LITERAL;
                    default: return ERROR_CODE;
                }
        }
    }
}

// TODO maybe add raw strings
int stringsAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0:
                switch(readChar) {
                    case '"': return TOKEN_STRING_LITERAL;
                    case '\\': status = 1; break;
                    case '\n': return ERROR_CODE;
                }
                break;
            case 1:
                for (int i = 0; i < SIZE_STRING_VALID_ESCAPED_CHARS; ++i) {
                    if(readChar == ARR_STRING_VALID_ESCAPED_CHARS[i]) {
                        status = 0;
                        break;
                    }
                }

                if(status == 1) return ERROR_CODE;
                break;
        }
    }
}

int numbersAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    resetFrontPosition(&(*lexicalAnalyzer)->inputSystem);
    if(isImaginary(lexicalAnalyzer)) {
        return TOKEN_IMAGINARY_LITERAL;
    }

    resetFrontPosition(&(*lexicalAnalyzer)->inputSystem);
    if(isFloat(lexicalAnalyzer)) { // TODO already called in isImaginary, maybe make more efficient
        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
        return TOKEN_FLOATING_POINT_LITERAL;
    }

    resetFrontPosition(&(*lexicalAnalyzer)->inputSystem);
    if(isInteger(lexicalAnalyzer)) {
        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
        return TOKEN_INTEGER_LITERAL;
    }

    resetFrontPosition(&(*lexicalAnalyzer)->inputSystem);
    nextChar(&(*lexicalAnalyzer)->inputSystem);

    return recognizeOperator(lexicalAnalyzer);
}


int mainAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    char readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

    if(isalpha(readChar) || readChar == '_') return alphanumericAutomaton(lexicalAnalyzer); // TODO find out if '_' is different
    if(readChar == '/') return commentsAutomaton(lexicalAnalyzer);
    if(readChar == '\'') return runesAutomaton(lexicalAnalyzer);
    if(readChar == '"') return stringsAutomaton(lexicalAnalyzer);
    if(isdigit(readChar) || readChar == '.') return numbersAutomaton(lexicalAnalyzer);
    if(readChar == ' ' || readChar == '\t' || readChar == '\r') return TOKEN_BLANK;
    if(readChar == '\n' || readChar == EOF) return readChar;

    return recognizeOperator(lexicalAnalyzer);
}

short isInteger(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0:
                if(readChar == '0') status = 2;
                else if(isdigit(readChar)) status = 1;
                else return 0;
                break;
            case 1:
                if(!isdigit(readChar)) return 1;
                break;
            case 2:
                if(isOctal(readChar)) status = 3;
                else if(readChar == 'x' || readChar == 'X') status = 4;
                else return 1;
                break;
            case 3:
                if(!isOctal(readChar)) return 1;
                break;
            case 4:
                if(isxdigit(readChar)) status = 5;
                else {
                    moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                    return 1;
                }
                break;
            case 5:
                if(!isxdigit(readChar)) return 1;
                break;
        }
    }
}

short isFloat(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0:
                if(isdigit(readChar)) status = 1;
                else if(readChar == '.') status = 6;
                else return 0;
                break;
            case 1:
                if(readChar == '.') status = 2;
                else if(readChar == 'e' || readChar == 'E') status = 3;
                else if(!isdigit(readChar)) return 0;
                break;
            case 2:
                if(readChar == 'e' || readChar == 'E') status = 3;
                else if(!isdigit(readChar)) return 1;
                break;
            case 3:
                if(readChar == '+' || readChar == '-') status = 4;
                else if(isdigit(readChar)) status = 5;
                else {
                    moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                    return 1;
                }
                break;
            case 4:
                if(isdigit(readChar)) status = 5;
                else {
                    moveBack(&(*lexicalAnalyzer)->inputSystem, 2);
                    return 1;
                }
                break;
            case 5:
                if(!isdigit(readChar)) return 1;
                break;
            case 6:
                if(isdigit(readChar)) status = 7;
                else return 0;
                break;
            case 7:
                if(readChar == 'e' || readChar == 'E') status = 3;
                else if(!isdigit(readChar)) return 1;
                break;
        }
    }
}

short isImaginary(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;
    short hasFloatFormat = isFloat(lexicalAnalyzer);

    if (hasFloatFormat) {
        moveBack(&(*lexicalAnalyzer)->inputSystem, 2);
    } else {
        resetFrontPosition(&(*lexicalAnalyzer)->inputSystem);
    }

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0:
                if(hasFloatFormat) status = 1;
                else if(isdigit(readChar)) status = 2;
                else return 0;
                break;
            case 1:
                if(readChar == 'i') return 1;
                else return 0;
            case 2:
                if(readChar == 'i') return 1;
                else if(!isdigit(readChar)) return 0;
        }
    }
}

int recognizeOperator(LexicalAnalyzer* lexicalAnalyzer) {
    char readOperator[MAXIMUM_OPERATOR_LENGTH + 1];
    int operatorId = ERROR_CODE;
    int newOperatorId;

    while(1) {
        getReadToken(&(*lexicalAnalyzer)->inputSystem, readOperator);
        newOperatorId = findHash(&(*lexicalAnalyzer)->operatorsTable, readOperator);

        if(newOperatorId == TOKEN_NOT_FOUND) {
            break;
        }

        operatorId = newOperatorId;
        nextChar(&(*lexicalAnalyzer)->inputSystem);
    }

    if (operatorId != ERROR_CODE) {
        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
    }

    return operatorId;
}

short isOctal(char digit) {
    return digit >= '0' && digit <= '7';
}