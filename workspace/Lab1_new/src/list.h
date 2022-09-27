#ifndef LIST_H
#define LIST_H

typedef struct ListHandle_t ListHandle_t;

struct ListHandle_t {
	ListHandle_t* prev;
	ListHandle_t* next;
};

typedef ListHandle_t List_t;

void addToList(List_t* list, ListHandle_t* item);

void removeFromList(ListHandle_t* item);

#endif
