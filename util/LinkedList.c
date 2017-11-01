#include <stdlib.h>
#include <string.h>
#include "../Definitions.h"

typedef struct {
    char* key;
    SymbolsTableValue value;
} ListItemStruct;

typedef ListItemStruct* ListItem;

typedef struct listNode {
    ListItem item;
    struct listNode* next;
} ListNodeStruct;

typedef ListNodeStruct* ListNode;

typedef struct {
    ListNode first;
    ListNode last;
} LinkedListStruct;

typedef LinkedListStruct* LinkedList;

/**
 * Crea una lista enlazada vacía. Las listas enlazadas se usan en la tabla hash para gestionar colisiones mediante encadenamiento
 * @param list La lista
 */
void createList(LinkedList* list){
    *list = (LinkedList) malloc(sizeof(LinkedListStruct));

    (*list)->first = NULL;
    (*list)->last = NULL;
}

/**
 * Devuelve el primer nodo de la lista
 * @param list La lista
 * @return El primer nodo
 */
ListNode firstNode(LinkedList* list){
    return (*list)->first;
}

/**
 * Devuelve el siguiente nodo de un nodo dado
 * @param baseNode El nodo del cual se quiere conocer su sucesor
 * @return El nodo sucesor
 */
ListNode nextNode(ListNode baseNode) {
    return baseNode->next;
}

/**
 * Obtiene el valor propiamente dicho de un nodo de la lista, el elemento que almacena la lista de manera útil
 * @param node El nodo
 * @return El valor
 */
ListItem nodeItem(ListNode node) {
    return node->item;
}

/**
 * Crea un ítem de la lista. Los ítems contienen una clave y un valor, que la tabla hash necesita para funcionar
 * @param item El ítem a crear
 * @param key La clave
 * @param value El valor
 */
void createItem(ListItem* item, char* key, SymbolsTableValue value) {
    *item = (ListItem) malloc(sizeof(ListItemStruct));

    (*item)->key = (char *) malloc((strlen(key) + 1) * sizeof(char));

    strcpy((*item)->key, key);
    (*item)->value = value;
}

/**
 * Destruye un ítem, liberando todos los recursos
 * @param item El ítem a destruir
 */
void destroyItem(ListItem* item) {
    free((*item)->key);
    free(*item);
}

/**
 * Obtiene la clave de un item
 * @param item El item
 * @return La clave
 */
char* itemKey(ListItem item){
    return item->key;
}

/**
 * Obtiene el valor de un item
 * @param item El item
 * @return El valor
 */
SymbolsTableValue itemValue(ListItem item){
    return item->value;
}

/**
 * Añade un nuevo item al final de la lista
 * @param list La lista
 * @param item El item a añadir
 */
void append(LinkedList* list, ListItem item){
    ListNode newNode = (ListNode) malloc(sizeof(ListNodeStruct));

    newNode->item = item;
    newNode->next = NULL;

    if((*list)->last == NULL) {
        (*list)->first = newNode;
    } else {
        (*list)->last->next = newNode;
    }

    (*list)->last = newNode;
}

/**
 * Destruye la lista, liberando todos los recursos
 * @param list La lista
 */
void destroyList(LinkedList* list){
    ListNode currentNode;

    while((*list)->first != NULL) {
        currentNode = (*list)->first;
        (*list)->first = (*list)->first->next;

        destroyItem(&currentNode->item);
        free(currentNode);
    }

    (*list)->first = (*list)->last = NULL;
    free(*list);
}
