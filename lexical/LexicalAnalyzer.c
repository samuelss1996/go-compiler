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

// TODO booleans
// TODO a float can begin with '.'
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
    int status = 0;
    char readChar;

    while(1) {
        readChar = nextChar(inputSystem);

        switch(status) {
            case 0: // Initial status
                if(isalpha(readChar) || readChar == '_') { // TODO find if '_' has a special meaning itself
                    return alphanumericAutomaton();
                } else if(readChar == '/') {
                    return commentsAutomaton();
                } else if(readChar == '\'') {
                    return runesAutomaton();
                } else if(readChar == '"') {
                    return stringsAutomaton();
                } else if(readChar == '0') {
                    status = 11;
                } else if(isdigit(readChar)) {
                    status = 20;
                } else if(readChar == ' ' || readChar == '\t' || readChar == '\r') {
                    return TOKEN_COMMENT; // TODO
                } else if(readChar == '\n' || readChar == EOF){
                    return readChar;
                } else {
                    return recognizeOperator();
                }
                break;
            case 11: // Recognized '0'. Expecting octal or hexadecimal integer, floating-point decimal, or imaginary
                if(readChar == 'x' || readChar == 'X') { // hexadecimal
                    status = 12;
                } else if(readChar >= '0' && readChar <= '7') { // octal
                    status = 14;
                } else if(isdigit(readChar)) { // float or imaginary
                    status = 15;
                } else if(readChar == 'i'){
                    return TOKEN_IMAGINARY_LITERAL;
                } else if(readChar == '.') {
                    status = 16;
                } else if(readChar == 'e' || readChar == 'E') {
                    status = 17;
                } else {
                    moveBack(inputSystem, 1);
                    return TOKEN_INTEGER_LITERAL;
                }
                break;
            case 12: // Recognized '0x'. Expecting at least one hexadecimal digit.
                if(isxdigit(_toupper(readChar))) {
                    status = 13;
                } else {
                    return ERROR_CODE;
                }
                break;
            case 13: // Recognized '0xF', where F is an hexadecimal digit. Expecting 0 or more hexadecimal digits
                if(!isxdigit(_toupper(readChar))) {
                    moveBack(inputSystem, 1);
                    return TOKEN_INTEGER_LITERAL; // TODO maybe change to hexadecimal
                }
                break;
            case 14: // Recognized '07', where 7 is an octal digit. Expecting 0 or more octal digits, float or imaginary number
                if(readChar == 'i') {
                    return TOKEN_IMAGINARY_LITERAL; // TODO can also be a decimal int to make an imaginary or float
                } else if(readChar == '8' || readChar == '9') {
                    status = 15;
                } else if(!isdigit(readChar)) {
                    moveBack(inputSystem, 1);
                    return TOKEN_INTEGER_LITERAL; // TODO maybe change to octal
                }
                break;
            case 15: // Recognized a number starting with '0', with no 'X' and at least one non-octal digit. Expecting imaginary or float
                if(readChar == 'i') {
                    return TOKEN_IMAGINARY_LITERAL;
                } else if(readChar == '.') {
                    status = 16;
                } else if(readChar == 'e' || readChar == 'E') {
                    status = 17;
                } else if(!isdigit(readChar)) {
                    return ERROR_CODE;
                }

                break;
            case 16: // Recognized numbers and a point ('.'). Expecting decimal part of float or imaginary
                if(readChar == 'i') {
                    return TOKEN_IMAGINARY_LITERAL; // TODO maybe change to float imaginary
                } else if(readChar == 'e' || readChar == 'E') {
                    status = 17;
                } else if(!isdigit(readChar)) {
                    moveBack(inputSystem, 1);
                    return TOKEN_FLOATING_POINT_LITERAL;
                }
                break;
            case 17: // Recognized number and 'e'. Expecting exponent
                if(readChar == '+' || readChar == '-') {
                    status = 18;
                } else if(isdigit(readChar)) {
                    status = 19;
                } else {
                    return ERROR_CODE;
                }
                break;
            case 18: // Recognized number and 'e' and '+' or '-'. Expecting exponent continuation
                if(isdigit(readChar)) {
                    status = 19;
                } else {
                    return ERROR_CODE;
                }
                break;
            case 19: // Recognized valid number exponent. Expecting more numbers or 'i' for imaginary
                if(readChar == 'i') {
                    return TOKEN_IMAGINARY_LITERAL; // TODO maybe change to float exponential imaginary
                } else if(!isdigit(readChar)) {
                    moveBack(inputSystem, 1);
                    return TOKEN_FLOATING_POINT_LITERAL; // TODO maybe change to float with exponent
                }
                break;
            case 20: // Recognized non-zero digit. Expecting decimal integer, float or imaginary
                if(readChar == 'i') {
                    return TOKEN_IMAGINARY_LITERAL;
                } else if(readChar == '.') {
                    status = 16;
                } else if(readChar == 'e' || readChar == 'E') {
                    status = 17;
                } else if(!isdigit(readChar)) {
                    moveBack(inputSystem, 1);
                    return TOKEN_INTEGER_LITERAL;
                }

                break;
        }
    }
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

void destroyLexicalAnalyzer() {
    destroyHashTable(&operatorsTable);
}