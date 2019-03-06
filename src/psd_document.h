#ifndef PSDUMP_DOCUMENT_H
#define PSDUMP_DOCUMENT_H


/*
This class serves as a root group containing all layers and groups
parsed from input files. It also holds some relevant meta-data such as
dimensions of the document and its filename.
*/
struct psd_document;

struct psd_document * psd_document_new(const char *name, int width, int height);
void psd_document_free(struct psd_document * doc);

void psd_document_save_layers(const struct psd_document * doc, const char *dir);

int psd_document_width(const struct psd_document * doc);
int psd_document_height(const struct psd_document * doc);

#endif // PSDUMP_DOCUMENT_H
