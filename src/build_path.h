#ifndef PSDUMP_BUILD_PATH_H
#define PSDUMP_BUILD_PATH_H

#include <string>

typedef std::string::size_type size_type;
std::string build_path(const char *dirname, const char *filename, const char *format);

#endif // PSDUMP_BUILD_PATH_H
