#include "psd_parser.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "psd_document.h"

static void psd_parser_reset_layer_index();
static int psd_parser_layer_index_is_valid();
static void psd_parser_next_layer_index();
static psd_layer_record * psd_parser_extract_layer_record();
static void psd_parser_parse_psd(psd_layer_group *);

struct psd_parser {
	char * filename;
	psd_context * context;
	int layer_index;
};

struct psd_parser * psd_parser_new(const char * filename) {
	if (filename == NULL)
		return NULL;
	int len = strlen(filename);
	if (len < 0)
		return NULL;

	struct psd_parser * parser = malloc(sizeof(struct psd_parser));
	if (parser == NULL)
		return NULL;

	parser->context = NULL;
	parser->layer_index = 0;

	parser->filename = malloc(len+1);
	if (parser->filename == NULL) {
		free(parser);
		return NULL;
	}
	parser->filename[len] = '\0';
	strcpy(parser->filename, filename);

	return parser;
}

void psd_parser_free(struct psd_parser * parser) {
	if (parser == NULL)
		return;

	free(parser->filename);
	free(parser);
}

struct psd_document * psd_parser_parse(struct psd_parser * parser) {
	if (parser == NULL)
		return NULL;

	psd_status status = psd_image_load(&parser->context, (psd_char *)parser->filename);

	if (status != psd_status_done)
	{
		// Failed to load the file
		return NULL;
	}

	struct psd_document *doc = psd_document_new(parser->filename, parser->context->width, parser->context->height);

	psd_parser_reset_layer_index(parser);
	psd_parser_parse_psd(parser, doc);

	psd_image_free(parser->context);
	parser->context = NULL;

	return doc;
}


/* A bunch of utility methods */

static int _is_group(const psd_layer_record *record)
{
  return record->layer_type == psd_layer_type_folder;
}

static int _is_end_of_group(const psd_layer_record *record)
{
  return record->layer_type == psd_layer_type_hidden
    && strcmp((const char *)record->layer_name, "</Layer group>") == 0;
}

static int _is_layer(const psd_layer_record *record)
{
  return record->layer_type == psd_layer_type_normal;
}

/* Helper functions */
static void psd_parser_reset_layer_index(struct psd_parser * parser)
{
  parser->layer_index = parser->context->layer_count - 1;
}

static int psd_parser_layer_index_is_valid(const struct psd_parser * parser)
{
  return parser->layer_index >= 0;
}

static void psd_parser_next_layer_index(struct psd_parser * parser)
{
  --parser->layer_index;
}

static psd_layer_record * psd_parser_extract_layer_record(const struct psd_parser * parser)
{
  assert(psd_parser_layer_index_is_valid(parser));

  return &parser->context->layer_records[parser->layer_index];
}

static void psd_parser_parse_psd(const struct psd_parser * parser, struct psd_layer_group * parent)
{
  while (psd_parser_layer_index_is_valid(parser)) {
    psd_layer_record *record = psd_parser_extract_layer_record();

    if (psd_parser_is_layer(record)) {
      Layer *layer = new Layer((const char *)record->layer_name, record->left, record->top, record->width, record->height);
	  layer->set_contents(record->image_data, record->width * record->height * 4);
      parent->add_child(layer);
    } else if (_is_group(record)) {
      LayerGroup *group = new LayerGroup((const char*)record->layer_name);
      parent->add_child(group);

      // This will recursively parse file hierarchy
      psd_parser_next_layer_index(parser);
      psd_parser_parse_psd(parser, group);
    } else if (psd_parser_is_end_of_group(record)) {
      // Do nothing in particular
      return;
    }

    psd_parser_next_layer_index(parser);
  }
}

