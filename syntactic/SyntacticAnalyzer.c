#include <stdlib.h>
#include <string.h>
#include "../lexical/LexicalAnalyzer.h"

typedef struct {
    LexicalAnalyzer lexicalAnalyzer;
} SyntacticAnalyzerStruct;

typedef SyntacticAnalyzerStruct* SyntacticAnalyzer;

void createSyntacticAnalyzer(SyntacticAnalyzer* syntacticAnalyzer, LexicalAnalyzer lexicalAnalyzer) {
    *syntacticAnalyzer = (SyntacticAnalyzer) malloc(sizeof(SyntacticAnalyzerStruct));
    (*syntacticAnalyzer)->lexicalAnalyzer = lexicalAnalyzer;
}

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
}

void destroySyntacticAnalyzer(SyntacticAnalyzer* syntacticAnalyzer) {
    free(*syntacticAnalyzer);
}