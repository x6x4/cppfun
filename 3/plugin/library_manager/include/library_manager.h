#ifndef LIBRARY_MANAGER_H
#define LIBRARY_MANAGER_H

#include <memory>
#include <dlfcn.h>
#include <vector>
#include <filesystem>

#include "../../dynamic_library/include/dynamic_library.h"

namespace fs = std::filesystem;

class LibraryManager {
  public:

  void scan(const std::string& path, const std::string& extension);

  void clear();

  const std::vector<DynamicLibrary>& getLibraries() const;

  private:

  std::vector<DynamicLibrary> libraries;
};

#endif // PLUGIN_MANAGER_H
