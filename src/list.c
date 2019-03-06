#include "list.h"

//struct Node {
//	struct psd_record * record;
//	struct Node * next;
//};

struct List {
	struct Node * first;
	int count;
	void (*free_func)(void *);
};

void * node_data(const Node * node) {
	if (node == NULL)
		return NULL;
	return node->data;
}

List * list_new(void (*free_func)(void *)) {
	List * list = malloc(sizeof(List));
	if (list == NULL)
		return NULL;
	list->first = NULL;
	list->count = 0;
	list->free_func = free_func;
	return list;
}

void list_free(List * list) {
	if (list == NULL)
		return;
	Node * node = list->first;
	if (list->free_func != NULL) {
		while (node != NULL) {
			Node * next = node->next;
			list->free_func(node);
			node = next;
		}
	}
	free(list);
}

int list_count(const List * list) {
	if (list == NULL)
		return -1;
	return list->count;
}

const Node * list_first(const List * list) {
	if (list == NULL)
		return NULL;
	return list->first;
}
