#pragma once

#include <algorithm>
#include <stdexcept>

enum state {
    EMPTY,
    PART,
    FULL
};

void check_cin ();


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

    Vector (Vector &&v) noexcept {
        swap (v);
        ~v;
    }

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

    void resize () {
        if (state() == FULL) {
            cap*=2;
            T* new_data = new T[cap];
            std::move(begin(), end(), new_data);
            delete [] data;
            data = new_data;
        }
    }

//  GETTERS

    T* begin () const noexcept { return data; }
    T*   end () const noexcept { return data + sz; }

    std::size_t capacity() const noexcept { return cap; }
    std::size_t size()     const noexcept { return sz;  }


//  DEVELOPERS-ONLY

private:

    std::size_t cap = 0x10;
    std::size_t sz = 0;
    T* data = new T[cap];
};

