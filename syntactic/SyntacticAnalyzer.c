#include <stdlib.h>
#include <string.h>
#include "../lexical/LexicalAnalyzer.h"
#include "../flex/lex.yy.h"

typedef struct {
    LexicalAnalyzer lexicalAnalyzer;
} SyntacticAnalyzerStruct;

typedef SyntacticAnalyzerStruct* SyntacticAnalyzer;

/**
 * Crea e inicializa el analizador sintáctico
 * @param syntacticAnalyzer El analizador sintáctico
 * @param lexicalAnalyzer El analizador léxico a emplear
 */
void createSyntacticAnalyzer(SyntacticAnalyzer* syntacticAnalyzer, LexicalAnalyzer lexicalAnalyzer) {
    *syntacticAnalyzer = (SyntacticAnalyzer) malloc(sizeof(SyntacticAnalyzerStruct));
    (*syntacticAnalyzer)->lexicalAnalyzer = lexicalAnalyzer;
}

/**
 * Comienza en análisis sintáctico. Esta función es la que se invoca desde el main para comenzar la compilación
 * @param syntacticAnalyzer El analizador sintáctico
 */
void startSyntacticAnalyzer(SyntacticAnalyzer* syntacticAnalyzer) {
    LexicalComponent component;
    int componentId;
    char* token, special[4];

    do {
        component = nextLexicalComponent(&(*syntacticAnalyzer)->lexicalAnalyzer);
        componentId = getLexicalComponentId(&component);
        token = getLexicalComponentToken(&component);

        if(componentId == '\n' || componentId == EOF) {
            strcpy(special, (componentId == EOF)? "EOF" : "\\n");
            token = special;
        }

        printf("%s -> %d\n", token, componentId);
        destroyLexicalComponent(&component);
    } while(componentId != EOF);

    yyin = fopen("concurrentSum.go", "r");
    printf("%d", yylex());
}

/**
 * Destruir el analizador sintáctico, liberando todos los recursos
 * @param syntacticAnalyzer El analizador sintáctico
 */
void destroySyntacticAnalyzer(SyntacticAnalyzer* syntacticAnalyzer) {
    free(*syntacticAnalyzer);
}