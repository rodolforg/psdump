#ifndef PSDUMP_LAYER_H
#define PSDUMP_LAYER_H


/*
This struct encapsulates all the relevant data for a layer.
*/
typedef struct psd_record psd_layer;

psd_layer * psd_layer_new(const char *name, int x, int y, int width, int height);
void psd_layer_free_data(void * data);

int psd_layer_x(const psd_layer * layer);
int psd_layer_y(const psd_layer * layer);
int psd_layer_width(const psd_layer * layer);
int psd_layer_height(const psd_layer * layer);

void psd_layer_set_contents(psd_layer * layer, unsigned int *image_data, unsigned long num_bytes);
void psd_layer_save_to_file(const psd_layer * layer, const char * path);

#endif // PSDUMP_LAYER_H
