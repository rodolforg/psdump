#ifndef PSDUMP_LAYERGROUP_H
#define PSDUMP_LAYERGROUP_H

#include "psd_record.h"

/*
This struct serves as a container for layers and other groups of layers.

Important: each child must point to a heap-allocated instance of psd_record
or one of its subclasses. Each LayerGroup instance owns its
children. Thus, when a LayerGroup instance is deleted, all of its
children are also automatically deleted by the LayerGroup instance.
*/
struct psd_layer_group;

struct psd_layer_group * psd_layer_group_new();
struct psd_layer_group * psd_layer_group_new_named(const char *name);
void psd_layer_group_free_data(void * data);

void psd_layer_group_add_child(struct psd_layer_group * layer_group, struct psd_record * child);

// These three methods provide iteration facility
struct psd_record * psd_layer_group_first_child(struct psd_layer_group * layer_group);
struct psd_record * psd_layer_group_next_child(struct psd_layer_group * layer_group);
int psd_layer_group_has_next_child(const struct psd_layer_group * layer_group);
int psd_layer_group_children_count(const struct psd_layer_group * layer_group);

#endif // PSDUMP_LAYERGROUP_H
