#include <stdlib.h>
#include <string.h>

typedef struct {
    int componentId;
    char* componentToken;
} LexicalComponentStruct;

typedef LexicalComponentStruct* LexicalComponent;

void createLexicalComponent(LexicalComponent* lexicalComponent, int componentId, char* componentToken) {
    *lexicalComponent = (LexicalComponent) malloc(sizeof(LexicalComponentStruct));

    (*lexicalComponent)->componentId = componentId;
    (*lexicalComponent)->componentToken = (char *) malloc((strlen(componentToken) + 1) * sizeof(char));

    strcpy((*lexicalComponent)->componentToken, componentToken);
}

int getLexicalComponentId(LexicalComponent* lexicalComponent) {
    return (*lexicalComponent)->componentId;
}

char* getLexicalComponentToken(LexicalComponent* lexicalComponent) {
    return (*lexicalComponent)->componentToken;
}

void destroyLexicalComponent(LexicalComponent* lexicalComponent) {
    free((*lexicalComponent)->componentToken);
    free(*lexicalComponent);
}