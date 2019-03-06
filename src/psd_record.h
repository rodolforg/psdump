#ifndef PSDUMP_RECORD_H
#define PSDUMP_RECORD_H

enum psd_record_type {
	PSD_RECORD_TYPE_UNKNOWN,
	PSD_RECORD_TYPE_LAYER,
	PSD_RECORD_TYPE_LAYER_GROUP,
};

/*
Abstract base struct for subclasses which represent concrete layer
records and groups of layers.
*/
struct psd_record;

//struct psd_record * psd_record_new(void (*free_data_cb)(void*));
struct psd_record * psd_record_new_named(const char * name, enum psd_record_type type, void (*free_data_cb)(void*));
void psd_record_free(struct psd_record * record);

void psd_record_set_name(struct psd_record * record, const char * name);
const char * psd_record_name(const struct psd_record * record);

void * psd_record_data(const struct psd_record * record);

int psd_record_is_group(const struct psd_record * record);

enum psd_record_type psd_record_type(const struct psd_record * record);


#endif // PSDUMP_RECORD_H
