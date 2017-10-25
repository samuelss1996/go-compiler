#include <stdlib.h>
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


void createList(LinkedList* list){
    *list = (LinkedList) malloc(sizeof(LinkedListStruct));

    (*list)->first = NULL;
    (*list)->last = NULL;
}

ListNode firstNode(LinkedList* list){
    return (*list)->first;
}

ListNode nextNode(ListNode baseNode) {
    return baseNode->next;
}

ListItem nodeItem(ListNode node) {
    return node->item;
}

void createItem(ListItem* item, char* key, SymbolsTableValue value) {
    *item = (ListItem) malloc(sizeof(ListItemStruct));

    (*item)->key = key;
    (*item)->value = value;
}

void destroyItem(ListItem* item) {
    free(*item);
}

char* itemKey(ListItem item){
    return item->key;
}

SymbolsTableValue itemValue(ListItem item){
    return item->value;
}

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
