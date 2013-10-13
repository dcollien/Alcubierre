#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "types/List.h"


typedef struct listNode *listNodePtr_t;

struct listNode {
	any_t value;
	listNodePtr_t next;
	listNodePtr_t prev;
};

struct _list {
	listNodePtr_t first;
	listNodePtr_t last;
	unsigned int size;
};

static listNodePtr_t createNode(any_t value);

List new_List(void) {
	List list = malloc(sizeof(struct _list));
	assert(list != NULL);

	list->first = NULL;
	list->last = NULL;
	list->size = 0;

	return list;
}

void destroy_List(List list) {
	listNodePtr_t node, prevNode;

	node = list->first;
	while (node != NULL) {
		prevNode = node;
		node = node->next;
		free(prevNode);
	}

	free(list);
}

void prepend_List(List list, any_t value) {
	listNodePtr_t node = createNode(value);
	node->next = list->first;
	node->prev = NULL;

	if (list->first == NULL) {
		list->last = node;
	} else {
		list->first->prev = node;
	}
	list->first = node;
	++(list->size);
}

void append_List(List list, any_t value) {
	listNodePtr_t node = createNode(value);
	node->next = NULL;
	node->prev = list->last;

	if (list->last == NULL) {
		list->first = node;
	} else {
		list->last->next = node;
	}
	list->last = node;
	++(list->size);
}

any_t removeFirst_List(List list) {
	assert(list->first != NULL);

	any_t value = list->first->value;

	listNodePtr_t removedNode = list->first;
	list->first = removedNode->next;

	free(removedNode);

	if (list->first == NULL) {
		list->last = list->first;
	} else {
		list->first->prev = NULL;
	}

	--(list->size);

	return value;
}

any_t removeLast_List(List list) {
	assert(list->last != NULL);

	any_t value = list->last->value;

	listNodePtr_t removedNode = list->last;
	list->last = removedNode->prev;

	free(removedNode);

	if (list->last == NULL) {
		list->first = list->last;
	} else {
		list->last->next = NULL;
	}

	--(list->size);

	return value;
}

unsigned int size_List(List list) {
	return list->size;
}

static listNodePtr_t createNode(any_t value) {
	listNodePtr_t node = malloc(sizeof(struct listNode));
	assert(node != NULL);

	node->value = value;
	return node;
}
