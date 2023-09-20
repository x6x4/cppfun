#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>

enum state {
    EMPTY,
    PART,
    FULL
};



class Entry {
public:
    std::string name;

    Entry(const std::string name);
};

class SubMenu {
public:
    std::string name = "";

    SubMenu(std::string name) : name (name) {};

    SubMenu(std::string name, std::initializer_list<std::string> s) :
            name (name), entries(s) {};

    std::vector<SubMenu> submenus = {};
    std::vector<std::string> entries = {};

    void dialog ();
};

std::ostream &operator<<(std::ostream &os, SubMenu s);

class Menu {
public:
    std::vector<SubMenu> submenus;
    
};

void dialog (Menu m);

void check_is (const std::istream& is);

//  VECTOR

template<typename T> 
class Vector {

public:

//  CONSTRUCTORS

    Vector () {};

    Vector (std::initializer_list<T> lst) 
            : cap(lst.size()), sz(lst.size()), data(new T[lst.size()]) {
        std::copy(lst.begin(), lst.end(), data);
    };

    Vector (const Vector &v) : cap (v.cap), sz (v.sz), data (new T[v.cap]) {
        std::copy (v.begin(), v.end(), data);
    };

    //  source: https://en.cppreference.com/w/cpp/language/copy_assignment
    //  (non copy-and-swap idiom)
    auto operator= (const Vector &v) noexcept {
        // not a self-assignment
        if (this != &v) {
            // resource cannot be reused
            if (sz != v.size()) {
                delete [] data;
                data = new T[v.size()];
                sz = v.size();
            }
            std::copy(v.begin(), v.end(), data);
        }
    };

    Vector (Vector &&v) noexcept {
        swap (v);
    }

    Vector& operator= (Vector &&v) noexcept {
        swap (v);
        v.sz = 0;
        v.cap = INIT_CAP;
        
        return *this;
    };

    void swap (Vector<T> &v2) noexcept {
        std::swap (data, v2.data);
        std::swap (cap, v2.cap);
        std::swap (sz, v2.sz);
    }

//  DESTRUCTOR

    ~Vector() { delete[] data; data = nullptr; cap = 0; sz = 0; }

    const T &operator[] (std::size_t i) const { 
        if (data == nullptr)
            throw std::runtime_error ("Access to uninitialized data");
        return *(data + i); 
    }

    T &operator[] (std::size_t i) { 
        if (data == nullptr)
            throw std::runtime_error ("Access to uninitialized data");
        return *(data + i); 
    }

    int state () const noexcept {
        if (sz == 0)    return EMPTY; 
        if (sz == cap)  return FULL;
        else            return PART;
    }

    void inc_size () {sz++; }
    void dec_size () {sz--; }

    void resize (std::size_t n) {
        if (n < sz) throw std::runtime_error ("Shrinking is not allowed.");
        cap = n;
        T* new_data = new T[cap];
        std::move(begin(), end(), new_data);
        delete [] data;
        data = new_data;
    }

//  GETTERS

    T* begin () const noexcept { return data; }
    T*   end () const noexcept { return data + sz; }

    std::size_t capacity() const noexcept { return cap; }
    std::size_t size()     const noexcept { return sz;  }


//  DEVELOPERS-ONLY

private:

    static const int INIT_CAP = 0x10;
    std::size_t cap = INIT_CAP;
    std::size_t sz = 0;
    T* data = new T[cap];
};

