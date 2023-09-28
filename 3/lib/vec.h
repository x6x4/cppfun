
#pragma once

#include <algorithm>
#include <initializer_list>
#include <stdexcept>


enum state {
    EMPTY,
    PART,
    FULL
};


//  VECTOR

template<typename T> 
class Vector {

using const_iterator = const T*;

public:

//  CONSTRUCTORS

    Vector () {};

    Vector (std::initializer_list<T> lst) 
            : cap(lst.size()), sz(lst.size()), data(new T[lst.size()]) {
        std::copy(lst.begin(), lst.end(), data);
    };

    auto operator= (const Vector &v) {
        
        if (this != &v) {
            if (sz != v.size()) {

                T* new_data = new T[v.size()];
                delete [] data;
                data = new_data;
                sz = v.size();
                cap = v.capacity();
            }
            std::copy(v.begin(), v.end(), data);
        }
    };

    Vector (const Vector &v) {
        *this = v;
    };

    auto operator= (Vector &&v) noexcept {
        if (this != &v) {
            swap (v);
            v.sz = 0;
            v.cap = INIT_CAP;
        }
        return *this;
    }

    Vector (Vector &&v) noexcept {
        *this = v;
    }

//  DESTRUCTOR

    ~Vector() { delete[] data; data = nullptr; cap = 0; sz = 0; }

    const T &operator[] (std::size_t i) const { 
        if (data == nullptr)
            throw std::runtime_error ("Access to uninitialized data");
        if (i > sz)
            throw std::logic_error ("Access out of bounds");
        
        return *(data + i); 
    }

    T &operator[] (std::size_t i) { 
        if (data == nullptr)
            throw std::runtime_error ("Access to uninitialized data");
        if (i > sz)
            throw std::logic_error ("Access out of bounds");
        
        return *(data + i); 
    }

    //  return enum 
    int state () const noexcept {
        if (sz == 0)    return EMPTY; 
        if (sz == cap)  return FULL;
        else            return PART;
    }

    void push_back (const T &item) {
        if (!data) 
            throw std::runtime_error ("Access to uninitialized data");
        if (state() == FULL) resize(cap*2);

        *data[sz++] = item;
    }

    void pop_back () {
        if (!data) 
            throw std::runtime_error ("Access to uninitialized data");
        if (state() == EMPTY) 
            throw std::logic_error ("Vector is empty");

        sz--;
    }

    void resize (std::size_t n) {
        if (n < sz) throw std::runtime_error ("Shrinking is not allowed.");
        cap = n;
        
        T* new_data = new T[cap];
        std::move(begin(), end(), new_data);
        delete [] data;
        data = new_data;
    }

//  GETTERS

    const_iterator begin () const noexcept { return data; }
    const_iterator   end () const noexcept { return data + sz; }

    std::size_t capacity() const noexcept { return cap; }
    std::size_t size()     const noexcept { return sz;  }


//  DEVELOPERS-ONLY

private:

    static const int INIT_CAP = 0x10;
    std::size_t cap = INIT_CAP;
    std::size_t sz = 0;
    T* data = new T[cap];

    void swap (Vector<T> &v2) noexcept {
        std::swap (data, v2.data);
        std::swap (cap, v2.cap);
        std::swap (sz, v2.sz);
    }
};