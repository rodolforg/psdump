#ifndef PSDUMP_PSDPARSER_H
#define PSDUMP_PSDPARSER_H


#include "libpsd.h"

/*
This struct is responsible for parsing given Photoshop file and construct
a representation of its layer hierarchy in memory. The representation is
returned by the 'psd_parser_parse' function as an instance of psd_document struct.
*/
struct psd_parser;
struct psd_document;

struct psd_parser * psd_parser_new(const char * filename);
void psd_parser_free(struct psd_parser * parser);
struct psd_document * psd_parser_parse(struct psd_parser * parser);

#endif // PSDUMP_PSDPARSER_H
