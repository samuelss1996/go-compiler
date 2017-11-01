#include <stdlib.h>
#include <string.h>

typedef struct {
    int componentId;
    char* componentToken;
} LexicalComponentStruct;

typedef LexicalComponentStruct* LexicalComponent;

/**
 * Crea e inicializa un componente léxico, con su ID y su cadena de texto
 * @param lexicalComponent El componente léxico
 * @param componentId El ID
 * @param componentToken La cadena de texto
 */
void createLexicalComponent(LexicalComponent* lexicalComponent, int componentId, char* componentToken) {
    *lexicalComponent = (LexicalComponent) malloc(sizeof(LexicalComponentStruct));

    (*lexicalComponent)->componentId = componentId;
    (*lexicalComponent)->componentToken = (char *) malloc((strlen(componentToken) + 1) * sizeof(char));

    strcpy((*lexicalComponent)->componentToken, componentToken);
}

/**
 * Obtener el ID del componente léxico
 * @param lexicalComponent El componente léxico
 * @return El ID
 */
int getLexicalComponentId(LexicalComponent* lexicalComponent) {
    return (*lexicalComponent)->componentId;
}

/**
 * Obtener la cadena de texto del componente léxico, como referencia
 * @param lexicalComponent El componente léxico
 * @return La cadena de texto
 */
char* getLexicalComponentToken(LexicalComponent* lexicalComponent) {
    return (*lexicalComponent)->componentToken;
}

/**
 * Destruir el componente léxico, liberando todos los recursos
 * @param lexicalComponent El componente léxico
 */
void destroyLexicalComponent(LexicalComponent* lexicalComponent) {
    free((*lexicalComponent)->componentToken);
    free(*lexicalComponent);
}