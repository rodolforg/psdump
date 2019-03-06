#include "build_path.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char * build_path(const char *dirname, const char *filename, const char *format)
{
	const size_t dirname_length = strlen(dirname);

	const char * basename = strrchr(filename, '/');
	if (basename == NULL)
		basename = strrchr(filename, '\\');
	if (basename == NULL)
		basename = filename;
	else
		basename += 1;

	const char * extension = strrchr(filename, '.');
	if (extension == NULL)
		extension = basename + strlen(basename);
	int old_extension_length = strlen(extension) - 1;

	const size_t format_length = strlen(format);
	const size_t full_path_length = dirname_length + (dirname_length > 0 ? 1 : 0) + (extension - basename) + 1 + format_length + 1;

	char * full_path = malloc(full_path_length);
	if (full_path == NULL)
		return NULL;

	int extension_pos = 0;
	if (dirname_length > 0)
		extension_pos = sprintf(full_path, "%s/%s", dirname, basename);
	else
		extension_pos = sprintf(full_path, "%s", basename);
	sprintf(full_path + (extension_pos - old_extension_length -1), ".%s", format);

	return full_path;
}

int main(int argc, char ** argv) {
	char * path = build_path("a", "hoje.tt", "ola");
	puts(path);
	free(path);
	return 0;
}