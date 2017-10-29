#include <ctype.h>
#include "../Definitions.h"
#include "../util/HashTable.h"
#include "../input/InputSystem.h"
#include "../symbols/SymbolsTable.h"
#include "LexicalComponent.h"

#define SIZE_RUNE_VALID_ESCAPED_CHARS 9
#define SIZE_STRING_VALID_ESCAPED_CHARS 9

char ARR_RUNE_VALID_ESCAPED_CHARS[] = {'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '\''};
char ARR_STRING_VALID_ESCAPED_CHARS[] = {'a', 'b', 'f', 'n', 'r', 't', 'v', '\\', '"'};

HashTable operatorsTable;
InputSystem* inputSystem;

int mainAutomaton();
int recognizeOperator();
int alphanumericAutomaton();
int commentsAutomaton();
int runesAutomaton();
int stringsAutomaton();
int numbersAutomaton();
short isInteger();
short isFloat();
short isImaginary();
short isOctal(char digit);

// TODO booleans
void initLexicalAnalyzer(InputSystem* is) {
    FILE *operatorsDb = fopen("../db/operators.db", "r");
    char operator[MAXIMUM_OPERATOR_LENGTH + 1];
    int operatorId;
    inputSystem = is;

    createHashTable(&operatorsTable);

    while(fscanf(operatorsDb, "%s %d", operator, &operatorId) > 0) {
        insertHash(&operatorsTable, operator, operatorId);
    }

    fclose(operatorsDb);
}

LexicalComponent nextLexicalComponent() {
    int componentId = mainAutomaton();
    char componentToken[200]; // TODO
    LexicalComponent result;

    getReadToken(inputSystem, componentToken);
    confirmToken(inputSystem);

    if(componentId == TOKEN_COMMENT) {
        return nextLexicalComponent();
    }

    createLexicalComponent(&result, componentId, componentToken);

    return result;
}

int mainAutomaton() {
    char readChar = nextChar(inputSystem);

    if(isalpha(readChar) || readChar == '_') return alphanumericAutomaton(); // TODO find out if '_' is different
    if(readChar == '/') return commentsAutomaton();
    if(readChar == '\'') return runesAutomaton();
    if(readChar == '"') return stringsAutomaton();
    if(isdigit(readChar) || readChar == '.') return numbersAutomaton();
    if(readChar == ' ' || readChar == '\t' || readChar == '\r') return TOKEN_COMMENT; // TODO
    if(readChar == '\n' || readChar == EOF) return readChar;

    return recognizeOperator();
}

int alphanumericAutomaton() {
    char readChar = nextChar(inputSystem);
    char token[200]; // TODO
    int tokenId;

    while(isalnum(readChar) || readChar == '_') {
        readChar = nextChar(inputSystem);
    }

    moveBack(inputSystem, 1);
    getReadToken(inputSystem, token);

    tokenId = findSymbol(token);

    if(tokenId == TOKEN_NOT_FOUND) {
        addSymbol(token, TOKEN_IDENTIFIER);
        return TOKEN_IDENTIFIER;
    }
}

int commentsAutomaton() {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(inputSystem);

        switch(status) {
            case 0:
                switch(readChar) {
                    case '/': status = 1; break;
                    case '*': status = 2; break;
                    default:
                        moveBack(inputSystem, 1);
                        return recognizeOperator();
                }
                break;
            case 1:
                if(readChar == '\n') {
                    moveBack(inputSystem, 1);
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

int runesAutomaton() {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(inputSystem);

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
int stringsAutomaton() {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(inputSystem);

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

int numbersAutomaton() {
    resetFrontPosition(inputSystem);
    if(isImaginary()) {
        return TOKEN_IMAGINARY_LITERAL;
    }

    resetFrontPosition(inputSystem);
    if(isFloat()) { // TODO already called in isImaginary, maybe make more efficient
        moveBack(inputSystem, 1);
        return TOKEN_FLOATING_POINT_LITERAL;
    }

    resetFrontPosition(inputSystem);
    if(isInteger()) {
        moveBack(inputSystem, 1);
        return TOKEN_INTEGER_LITERAL;
    }

    resetFrontPosition(inputSystem);
    nextChar(inputSystem);

    return recognizeOperator();
}

short isInteger() {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(inputSystem);

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
                    moveBack(inputSystem, 1);
                    return 1;
                }
                break;
            case 5:
                if(!isxdigit(readChar)) return 1;
                break;
        }
    }
}

short isFloat() {
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(inputSystem);

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
                    moveBack(inputSystem, 1); // TODO test this please
                    return 1;
                }
                break;
            case 4:
                if(isdigit(readChar)) status = 5;
                else {
                    moveBack(inputSystem, 2);
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

short isImaginary() {
    int status = 0;
    char readChar;
    short hasFloatFormat = isFloat();

    if (!hasFloatFormat) {
        resetFrontPosition(inputSystem);
    } else {
        moveBack(inputSystem, 2);
    }

    while(1) {
        readChar = nextChar(inputSystem);

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

int recognizeOperator() {
    char readOperator[MAXIMUM_OPERATOR_LENGTH + 1];
    int operatorId = ERROR_CODE;
    int newOperatorId;

    while(1) {
        getReadToken(inputSystem, readOperator);
        newOperatorId = findHash(&operatorsTable, readOperator);

        if(newOperatorId == TOKEN_NOT_FOUND) {
            break;
        }

        operatorId = newOperatorId;
        nextChar(inputSystem);
    }

    if (operatorId != ERROR_CODE) {
        moveBack(inputSystem, 1);
    }

    return operatorId;
}

short isOctal(char digit) {
    return digit >= '0' && digit <= '7';
}

void destroyLexicalAnalyzer() {
    destroyHashTable(&operatorsTable);
}