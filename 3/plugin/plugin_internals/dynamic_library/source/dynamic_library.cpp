#include "dynamic_library.h"

DynamicLibrary::DynamicLibrary() noexcept: lib(nullptr) {}

DynamicLibrary::DynamicLibrary(const std::string& path): lib(nullptr) { 
    open(path); 
}

DynamicLibrary::~DynamicLibrary() { 
    close(); 
}

DynamicLibrary::DynamicLibrary(DynamicLibrary&& o):lib(o.lib)
{ 
    o.lib = nullptr; 
}

DynamicLibrary& DynamicLibrary::operator=(DynamicLibrary&& o)
{ 
    std::swap(lib, o.lib); return *this; 
}

void DynamicLibrary::open(const std::string& path) {
    if(lib != nullptr) {
        throw std::runtime_error("Already open");
    }
    lib = dlopen(path.c_str(), RTLD_NOW);
    if(lib == nullptr) {
        throw std::runtime_error(dlerror());
    }
}

void DynamicLibrary::close() noexcept {
    if(lib != nullptr) {
        dlclose(lib);
    }
    lib = nullptr;
}
