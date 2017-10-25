typedef void* LinkedList;
typedef void* ListNode;
typedef int ListItem;

void createList(LinkedList* list);
ListNode firstNode(LinkedList* list);
ListNode nextNode(LinkedList* list, ListNode baseNode);
ListItem nodeValue(LinkedList* list, ListNode node);
void append(LinkedList* list, ListItem item);
void destroyList(LinkedList* list);