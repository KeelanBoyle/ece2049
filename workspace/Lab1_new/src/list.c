#include <list.h>

void addToList(List_t* list, ListHandle_t* item) {
	item->next = list->next;
	item->next->prev = item;
	item->prev = list;
	list->next = item;
}

void removeFromList(ListHandle_t* item) {
	if(item->next != 0) {
		item->next->prev = item->prev;
	}

	if(item->prev != 0) {
		item->prev->next = item->next;
	}
}
