#include "../include/library_manager.h"

void LibraryManager::scan(const std::string& path, const std::string& extension) {
  fs::path dirPath(path);
  for(auto& file : fs::directory_iterator(dirPath)) {
    if((file.is_regular_file() || file.is_symlink()) && file.path().extension() == extension) {
      libraries.emplace_back(file.path());
    }
  }
}

void LibraryManager::clear() {
  libraries.clear();
}

const std::vector<DynamicLibrary>& LibraryManager::getLibraries() const {
  return libraries;
}
