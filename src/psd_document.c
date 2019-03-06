#include "psd_document.h"

#include "psd_layer_group.h"

#if !defined(_MSC_VER) && !defined(_WIN32) && !defined(__MINGW32__)
#  include <sys/stat.h>
#else
#  include <direct.h>
#  define mkdir(x, y) _mkdir(x)
#endif

#include "build_path.h"
#include "psd_record.h"
#include "psd_layer.h"

#include <stdlib.h>
#include <stdio.h>

struct psd_document {
	int width;
	int height;
	struct psd_layer_group * root;
};

struct psd_document * psd_document_new(const char *name, int width, int height) {
	struct psd_document * doc = malloc(sizeof(struct psd_document));
	if (doc == NULL)
		return NULL;
	doc->root = psd_layer_group_new_named(name);
	if (doc->root == NULL) {
		free(doc);
		return NULL;
	}
	doc->width = width;
	doc->height = height;
	return doc;
}

void psd_document_free(struct psd_document * doc) {
	if (doc == NULL)
		return;
	psd_record_free(doc->root);
	free(doc);
}

static void save_layers(const char * dir, struct psd_layer_group * group)
{
	struct psd_record *child = psd_layer_group_first_child(group);
	if (!child) return;

	mkdir(dir, 0755);

	do {
		if (psd_record_is_group(child)) {
			char path_prefix[2048];
			if (dir[0] != '\0')
				snprintf(path_prefix, 2047, "%s/%s", dir, psd_record_name(child));
			else
				snprintf(path_prefix, 2047, "%s", psd_record_name(child));
		    struct psd_layer_group *child_group = (struct psd_layer_group*)psd_record_data(child);
			if (psd_layer_group_children_count(child_group)) {
				save_layers(path_prefix, child_group);
			}
		} else {
			struct psd_layer * layer = (struct psd_layer *)psd_record_data(child);
			char * path = build_path(dir, psd_record_name(child), "png");
			psd_layer_save_to_file(layer, path);
			free(path);
	    }
	} while (child = psd_layer_group_next_child(group));
}

void psd_document_save_layers(const struct psd_document * doc, const char *dir) {
	if (doc == NULL || dir == NULL)
		return;
	save_layers(dir, doc->root);
}

int psd_document_width(const struct psd_document * doc) {
	if (doc == NULL)
		return -1;
	return doc->width;
}

int psd_document_height(const struct psd_document * doc) {
	if (doc == NULL)
		return -1;
	return doc->height;
}
