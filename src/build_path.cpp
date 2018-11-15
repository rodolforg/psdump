#include "build_path.h"

std::string build_path(const char *dirname, const char *filename, const char *format)
{
  std::string full_path(dirname);
  if (full_path.length() > 0)
	full_path += "/";

  std::string file_name(filename);
  size_type slash_index = file_name.rfind('/');
  if (slash_index == std::string::npos) {
    slash_index = file_name.rfind('\\');
  }
  if (slash_index != std::string::npos) {
    // Leave only filename
    file_name.erase(0, slash_index + 1);
  }

  size_type dot_index = file_name.rfind('.');
  if (dot_index == std::string::npos) {
	  file_name += ".";
	  file_name += format;
  } else {
	file_name.replace(dot_index + 1, file_name.size() - dot_index - 1, format);
  }

  full_path += file_name;

  return full_path;
}

