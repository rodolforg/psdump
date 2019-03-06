#include "psd_layer_group.h"

#include "psd_record.h"

#include "list.h"

#include <stdlib.h>

struct psd_layer_group {
	List * children;
	const Node * children_iter;
};

struct psd_layer_group * psd_layer_group_new_named(const char *name) {
	struct psd_layer_group * layer_group = malloc(sizeof(struct psd_layer_group));
	if (layer_group == NULL)
		return NULL;

	layer_group->data = psd_record_new_named(name, PSD_RECORD_TYPE_LAYER_GROUP, psd_layer_group_free_data);
	layer_group->children = list_new(psd_record_free);
	layer_group->children_iter = NULL;
	return layer_group;
}

void psd_layer_group_free_data(void * data) {
 	if (data == NULL)
		return;
	struct psd_layer_group * group = (struct psd_layer_group *) data;
	list_free(group->children);

	free(group);
}

void psd_layer_group_add_child(struct psd_layer_group * layer_group, struct psd_record * child) {
	if (layer_group == NULL || child == NULL || layer_group->children == NULL)
		return;
	list_append(layer_group->children, child);
}

// These three methods provide iteration facility
struct psd_record * psd_layer_group_first_child(struct psd_layer_group * layer_group) {
	layer_group->children_iter = list_first(layer_group->children);
	if (layer_group->children_iter == NULL)
		return NULL;
	return layer_group->children_iter->data;
}

struct psd_record * psd_layer_group_next_child(struct psd_layer_group * layer_group) {
	if (layer_group == NULL || layer_group->children_iter == NULL)
		return NULL;
	layer_group->children_iter = layer_group->children_iter->next;
	if (layer_group->children_iter == NULL)
		return NULL;
	return layer_group->children_iter->data;
}

int psd_layer_group_has_next_child(const struct psd_layer_group * layer_group) {
	if (layer_group == NULL || layer_group->children_iter == NULL)
		return 0;
	return layer_group->children_iter->next != NULL;
}

int psd_layer_group_children_count(const struct psd_layer_group * layer_group) {
	if (layer_group == NULL || layer_group->children_iter == NULL)
		return 0;
	return list_count(layer_group->children);
}
