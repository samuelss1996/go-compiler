#include <ctype.h>
#include <stdlib.h>
#include "../Definitions.h"
#include "../util/HashTable.h"
#include "../input/InputSystem.h"
#include "../symbols/SymbolsTable.h"
#include "LexicalComponent.h"
#include "../errors/Errors.h"

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

/**
 * Crear e incializar el analizador léxico
 * @param lexicalAnalyzer El analizador léxico
 * @param inputSystem El sistema de entrada a usar
 * @param symbolsTable  La tabla de símbolos a usar
 * @param operatorsTable La tabla de operadores a usar
 */
void createLexicalAnalyzer(LexicalAnalyzer *lexicalAnalyzer, InputSystem inputSystem, SymbolsTable symbolsTable, HashTable operatorsTable) {
    *lexicalAnalyzer = (LexicalAnalyzer) malloc(sizeof(LexicalAnalyzerStruct));

    (*lexicalAnalyzer)->inputSystem = inputSystem;
    (*lexicalAnalyzer)->symbolsTable = symbolsTable;
    (*lexicalAnalyzer)->operatorsTable = operatorsTable;
}

/**
 * Obtener el siguiente componente léxico.
 * @param lexicalAnalyzer El analizador léxico
 * @return El siguiente componente léxico.
 */
LexicalComponent nextLexicalComponent(LexicalAnalyzer* lexicalAnalyzer) {
    int componentId = mainAutomaton(lexicalAnalyzer);
    char *tokenAsString;
    LexicalComponent result;

    getReadToken(&(*lexicalAnalyzer)->inputSystem, &tokenAsString);
    confirmToken(&(*lexicalAnalyzer)->inputSystem);

    if(componentId == TOKEN_COMMENT || componentId == TOKEN_BLANK || componentId == ERROR_CODE) {
        free(tokenAsString);
        return nextLexicalComponent(lexicalAnalyzer);
    }

    createLexicalComponent(&result, componentId, tokenAsString);
    free(tokenAsString);

    return result;
}

/**
 * Destruir el analizador léxico, liberando los recursos
 * @param lexicalAnalyzer
 */
void destroyLexicalAnalyzer(LexicalAnalyzer* lexicalAnalyzer) {
    free(*lexicalAnalyzer);
}

/**
 * Autómata que reconoce las cadenas alfanuméricas. Cuando se invoca ya se ha leído un carácter alfabético
 * @param lexicalAnalyzer El analizador léxico
 * @return El identificador del componente léxico
 */
int alphanumericAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    char readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);
    char *tokenAsString;
    int tokenId;

    while(isalnum(readChar) || readChar == '_') {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);
    }

    moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
    getReadToken(&(*lexicalAnalyzer)->inputSystem, &tokenAsString);

    tokenId = findSymbol(&(*lexicalAnalyzer)->symbolsTable, tokenAsString);

    if(tokenId == TOKEN_NOT_FOUND) {
        addSymbol(&(*lexicalAnalyzer)->symbolsTable, tokenAsString, TOKEN_IDENTIFIER);
        free(tokenAsString);

        return TOKEN_IDENTIFIER;
    }

    free(tokenAsString);
    return tokenId;
}

/**
 * Autómata que reconoce los comentarios, tanto de una línea como multi-línea. Cuando se invoca ya se ha leído una '/'
 * Si encuentra al menos un salto de línea dentro de un comentario, el componente al completo se reconoce como un salto de línea
 * @param lexicalAnalyzer El analizador léxico
 * @return El identificador del componente léxico
 */
int commentsAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;
    int result = TOKEN_COMMENT;

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0: // Ya se ha leido un caracter '/' fuera del automata, se espera '/' o '*' si es un comentario.
                switch(readChar) {
                    case '/': status = 1; break;
                    case '*': status = 2; break;
                    default: // Si no es '*' ni '/', la '/' leida fuera del automata es una division
                        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                        return recognizeOperator(lexicalAnalyzer);
                }
                break;
            case 1: // Se ha leído '//'. Comentario de una línea. Termina en \n o EOF
                if(readChar == '\n' || readChar == EOF) {
                    moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                    return result;
                }
                break;
            case 2: // Se ha leido '/*' Es posible que el comentario termine al leer un '*'. Termina si se lee EOF
                switch(readChar) {
                    case '\n': result = '\n'; break;
                    case '*': status = 3; break;
                    case EOF:
                        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                        return result;
                }
                break;
            case 3: // Se ha leido '/* ... *'. Si se lee una '/' o un EOF el comentario se termina. En caso contrario el comentario sigue normalmente
                switch(readChar) {
                    case '/': return result;
                    case EOF:
                        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                        return result;
                    default: status = 2; break;
                }
                break;
        }
    }
}

/**
 * Automata que reconoce runes: caracteres encerrados entre comillas simples. Cuando se invoca ya se ha leido una comilla simple
 * Este automata no es neceario para reconocer el codigo dado, su presencia aquí se debe a un despiste
 * @param lexicalAnalyzer El analizador léxico
 * @return El identificador del componente léxico
 */
int runesAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;
    int result = TOKEN_RUNE_LITERAL;
    short multipleRead = 0;

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0: // Se ha leido una comilla simple. Se espera un carácter o una barra de escape
                switch(readChar) {
                    case '\\': status = 1; break;
                    case '\n':
                    case EOF:
                        expectingEndOfRune(getCurrentLine(&(*lexicalAnalyzer)->inputSystem), getCurrentColumn(&(*lexicalAnalyzer)->inputSystem));
                        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                        return ERROR_CODE;
                    case '\'':
                        emptyRune(getCurrentLine(&(*lexicalAnalyzer)->inputSystem), getCurrentColumn(&(*lexicalAnalyzer)->inputSystem));
                        return ERROR_CODE;
                    default: status = 2; break;
                }
                break;
            case 1: // Se ha leído una comilla simple y una barra de escape ('\). Se espera un carácter escapable válido
                for (int i = 0; i < SIZE_RUNE_VALID_ESCAPED_CHARS; ++i) {
                    if(readChar == ARR_RUNE_VALID_ESCAPED_CHARS[i]) {
                        status = 2;
                        break;
                    }
                }

                if(status == 1) {
                    invalidEscapedCharInsideRune(getCurrentLine(&(*lexicalAnalyzer)->inputSystem), getCurrentColumn(&(*lexicalAnalyzer)->inputSystem), readChar);

                    status = 2;
                    result = ERROR_CODE;
                }
                break;
            case 2: // Se ha leído una comilla simple y un caracter, o un caracter escapado ('a) ('\n). Se espera una comilla simple de cierre
                switch(readChar) {
                    case '\'': return result;
                    case '\n':
                    case EOF:
                        expectingEndOfRune(getCurrentLine(&(*lexicalAnalyzer)->inputSystem), getCurrentColumn(&(*lexicalAnalyzer)->inputSystem));
                        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                        return ERROR_CODE;
                    default:
                        if(!multipleRead) {
                            multiCharacterRune(getCurrentLine(&(*lexicalAnalyzer)->inputSystem), getCurrentColumn(&(*lexicalAnalyzer)->inputSystem));
                            multipleRead = 1;
                        }

                        result = ERROR_CODE;
                        break;
                }
        }
    }
}

/**
 * Automata que reconoce cadenas de texto. Cuando se invoca ya se ha leido una comilla doble (")
 * @param lexicalAnalyzer El analizador léxico
 * @return El identificador del componente léxico
 */
int stringsAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    int status = 0;
    char readChar;
    int result = TOKEN_STRING_LITERAL;

    while(1) {
        readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

        switch(status) {
            case 0: // Posición cualquiera dentro de la cadena. Se espera cualquier caracter válido. Si se lee " se cierra el string
                switch(readChar) {
                    case '"': return result;
                    case '\\': status = 1; break;
                    case '\n':
                    case EOF:
                        expectingEndOfString(getCurrentLine(&(*lexicalAnalyzer)->inputSystem), getCurrentColumn(&(*lexicalAnalyzer)->inputSystem));
                        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
                        return ERROR_CODE;
                }
                break;
            case 1: // Se ha leido una barra de escape '/' dentro del string. Se espera un caracter escapable válido
                for (int i = 0; i < SIZE_STRING_VALID_ESCAPED_CHARS; ++i) {
                    if(readChar == ARR_STRING_VALID_ESCAPED_CHARS[i]) {
                        status = 0;
                        break;
                    }
                }

                if(status == 1) {
                    invalidEscapedCharInsideString(getCurrentLine(&(*lexicalAnalyzer)->inputSystem), getCurrentColumn(&(*lexicalAnalyzer)->inputSystem), readChar);

                    status = 0;
                    result = ERROR_CODE;
                }
                break;
        }
    }
}

/**
 * Automata que reonoce los números: ennteros, flotantes e imaginarios. Llama a su vez a otros autómatas para realizar el reconocimiento
 * @param lexicalAnalyzer El analizador léxico
 * @return El identificador del componente léxico
 */
int numbersAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    resetFrontPosition(&(*lexicalAnalyzer)->inputSystem);
    switch(isImaginary(lexicalAnalyzer)) { // Determina si es imaginario, flotante, o ninguno de los dos
        case 1: return TOKEN_IMAGINARY_LITERAL; // Es imaginario
        case -1:
            moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
            return TOKEN_FLOATING_POINT_LITERAL; // Es flotante
    }

    resetFrontPosition(&(*lexicalAnalyzer)->inputSystem);
    if(isInteger(lexicalAnalyzer)) { // Determina si es entero
        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
        return TOKEN_INTEGER_LITERAL;
    }

    resetFrontPosition(&(*lexicalAnalyzer)->inputSystem);
    nextChar(&(*lexicalAnalyzer)->inputSystem);

    // Si no es ningún numero válido, puede que sea un operador, pues los números pueden empezar por '.', pero '.' puede ser operador
    return recognizeOperator(lexicalAnalyzer);
}

/**
 * Autómata principal. En función del primer carácter de un componente léxico invoca al autómata correspondiente
 * @param lexicalAnalyzer El analizador léxico
 * @return El identificador del componente léxico
 */
int mainAutomaton(LexicalAnalyzer* lexicalAnalyzer) {
    char readChar = nextChar(&(*lexicalAnalyzer)->inputSystem);

    if(isalpha(readChar) || readChar == '_') return alphanumericAutomaton(lexicalAnalyzer);
    if(readChar == '/') return commentsAutomaton(lexicalAnalyzer);
    if(readChar == '\'') return runesAutomaton(lexicalAnalyzer);
    if(readChar == '"') return stringsAutomaton(lexicalAnalyzer);
    if(isdigit(readChar) || readChar == '.') return numbersAutomaton(lexicalAnalyzer);
    if(readChar == ' ' || readChar == '\t' || readChar == '\r') return TOKEN_BLANK;
    if(readChar == '\n' || readChar == EOF) return readChar;

    return recognizeOperator(lexicalAnalyzer);
}

/**
 * Determina si el componente léxico es un número entero. Es importante comprobar antes si se trata de un imaginario o un flotante
 * @param lexicalAnalyzer El analizador léxico
 * @return 1 si es un entero, 0 si no lo es
 */
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

/**
 * Determina si el componente léxico es un número flotante. Es importante comprobar antes si se trata de un imaginario
 * @param lexicalAnalyzer El analizador léxico
 * @return 1 si es un flotante, 0 si no lo es
 */
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

/**
 * Determina si el componente léxico es un número imaginario, o un flotante (internamente llama a isFloat()).
 * @param lexicalAnalyzer El analizador léxico
 * @return 1 si es imaginario, -1 si es flotante, 0 si no es ninguno de los dos
 */
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
                else return -1;
            case 2:
                if(readChar == 'i') return 1;
                else if(!isdigit(readChar)) return 0;
        }
    }
}

/**
 * Reconoce si un componente léxico es un operador, y cuál es. Usa para ello la tabla de operadores
 * @param lexicalAnalyzer El analizador léxico
 * @return El identificador del operador
 */
int recognizeOperator(LexicalAnalyzer* lexicalAnalyzer) {
    char *readOperator;
    int operatorId = ERROR_CODE;
    int newOperatorId;

    while(1) {
        getReadToken(&(*lexicalAnalyzer)->inputSystem, &readOperator);
        newOperatorId = findHash(&(*lexicalAnalyzer)->operatorsTable, readOperator);

        if(newOperatorId == TOKEN_NOT_FOUND) {
            break;
        }

        operatorId = newOperatorId;
        nextChar(&(*lexicalAnalyzer)->inputSystem);

        free(readOperator);
    }

    if (operatorId != ERROR_CODE) {
        moveBack(&(*lexicalAnalyzer)->inputSystem, 1);
    } else {
        invalidSymbol(getCurrentLine(&(*lexicalAnalyzer)->inputSystem), getCurrentColumn(&(*lexicalAnalyzer)->inputSystem), *readOperator);
    }

    free(readOperator);
    return operatorId;
}

/**
 * Determina si un dígito es octal
 * @return Un valor distinto de cero si el dígito es octal
 */
short isOctal(char digit) {
    return digit >= '0' && digit <= '7';
}