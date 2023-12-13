#ifndef DYNAMIC_LIBRARY_H
#define DYNAMIC_LIBRARY_H

#include <memory>
#include <dlfcn.h>

class DynamicLibrary {
public:
    DynamicLibrary() noexcept;

    DynamicLibrary(const std::string& path);

    ~DynamicLibrary();

    // Нельзя копировать, иначе закроем 2 раза
    DynamicLibrary(const DynamicLibrary& o) = delete;
      
    DynamicLibrary&
    operator=(const DynamicLibrary& o) = delete;

    //Но можно перемещать
    DynamicLibrary(DynamicLibrary&& o);

    DynamicLibrary& operator=(DynamicLibrary&& o);

    void open(const std::string& path);

    void close() noexcept;

    template<typename T>
    T symbol(const std::string& name) const requires std::is_pointer_v<T>;

private:
    void* lib; // Лучше хранить умный казатель
};

template<typename T>
T DynamicLibrary::symbol(const std::string& name) const requires std::is_pointer_v<T> {
    auto ret = reinterpret_cast<T>(dlsym(lib, name.c_str()));
    if(ret == nullptr) {
        throw std::runtime_error(dlerror());
    }
    return ret;
}

#endif
