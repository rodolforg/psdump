#include "psd_record.h"

#include <string.h>
#include <stdlib.h>

struct psd_layer;
struct psd_layer_group;

struct psd_record {
	char * name;
	enum psd_record_type type;
	void * data;
	void (*free_data_cb)(void*);
};

struct psd_record * psd_record_new(void (*free_data_cb)(void*)) {
	struct psd_record * record = malloc(sizeof(struct psd_record));
	if (record == NULL)
		return NULL;
	record->name = NULL;
	record->type = PSD_RECORD_TYPE_UNKNOWN;
	record->data = NULL;
	record->free_data_cb = free_data_cb;
	return record;
}

struct psd_record * psd_record_new_named(const char * name, enum psd_record_type type, void (*free_data_cb)(void*)) {
	struct psd_record * record = psd_record_new(free_data_cb);
	if (record == NULL)
		return NULL;
	record->name = strdup(name);
	if (record->name == NULL) {
		free(record);
		return NULL;
	}
	record->type = type;
	return record;
}

void psd_record_free(struct psd_record * record) {
	if (record == NULL)
		return;
	if (record->name != NULL)
		free(record->name);
	if (record->free_data_cb != NULL)
		record->free_data_cb(record->data);
	free(record);
}

void psd_record_set_name(struct psd_record * record, const char * name) {
	if (record == NULL || name == NULL)
		return;
	if (record->name != NULL)
		free(record->name);
	record->name = strdup(name);
}

const char * psd_record_name(const struct psd_record * record) {
	if (record == NULL)
		return NULL;
	return record->name;
}

void * psd_record_data(const struct psd_record * record) {
	if (record == NULL)
		return NULL;
	return record->data;
}

int psd_record_is_group(const struct psd_record * record) {
	if (record == NULL)
		return 0;
	return record->type == PSD_RECORD_TYPE_LAYER_GROUP;
}

enum psd_record_type psd_record_type(const struct psd_record * record) {
	if (record == NULL)
		return PSD_RECORD_TYPE_UNKNOWN;
	return record->type;
}
