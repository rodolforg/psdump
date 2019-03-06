#include "psd_layer.h"

#include "psd_record.h"

#include <stdlib.h>
#include <string.h>

#include "lodepng/lodepng.h"

#define IS_LAYER(record) (psd_record_type(record) == PSD_RECORD_TYPE_LAYER)

struct psd_layer_data {
	int x;
	int y;
	int width;
	int height;

	unsigned int *image_data;
	unsigned long image_data_length;
};

psd_layer * psd_layer_new(const char *name, int x, int y, int width, int height) {
	struct psd_layer_data * layer_data = malloc(sizeof(struct psd_layer_data));
	if (layer_data == NULL)
		return NULL;

	psd_layer * record = psd_record_new_named(name, PSD_RECORD_TYPE_LAYER, psd_layer_free_data);
	if (record == NULL) {
		free(layer_data);
		return NULL;
	}

	layer_data->x = x;
	layer_data->y = y;
	layer_data->width = width;
	layer_data->height = height;

	layer_data->image_data = NULL;
	layer_data->image_data_length = 0;

	record->data = layer_data;

	return record;
}

void psd_layer_free_data(void * data) {
 	if (data == NULL)
		return;
	struct psd_layer * layer = (struct psd_layer *) data;
	free (layer->image_data);

	free(layer);
}

int psd_layer_x(const struct psd_layer * layer) {
	if (layer == NULL)
		return -1;
	return layer->x;
}

int psd_layer_y(const struct psd_layer * layer) {
	if (layer == NULL)
		return -1;
	return layer->y;
}

int psd_layer_width(const struct psd_layer * layer) {
	if (layer == NULL)
		return -1;
	return layer->width;
}

int psd_layer_height(const struct psd_layer * layer) {
	if (layer == NULL)
		return -1;
	return layer->height;
}

void psd_layer_set_contents(struct psd_layer * layer, unsigned int *image_data, unsigned long num_bytes) {
	if (layer == NULL)
		return;
	free(layer->image_data);

	layer->image_data = malloc(sizeof(unsigned int) * num_bytes);
	layer->image_data_length = num_bytes / 4;

	memcpy(layer->image_data, image_data, num_bytes);
}

void psd_layer_save_to_file(const struct psd_layer * layer, const char * path) {
	static const int PNG_COLOR_TYPE_RGB_ALPHA = 6;

	for (unsigned long i = 0; i < layer->image_data_length; ++i)
	{
		unsigned int pixel = layer->image_data[i];
		// convert ABGR to ARGB
		pixel = (pixel & 0xFF00FF00) | ((pixel << 16) & 0x00FF0000) | ((pixel >> 16) & 0x000000FF);
		//				A0G0					0R00							000B
		layer->image_data[i] = pixel;
	}

	LodePNG_encode_file(path, (unsigned char *)(layer->image_data), layer->width, layer->height, PNG_COLOR_TYPE_RGB_ALPHA, 8);
}