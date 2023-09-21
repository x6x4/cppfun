#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <utility>


enum State {
    EMPTY,
    PART,
    FULL
};

void check_is (const std::istream& is);


//  SET - ORDERED CONTAINER (KEY, COMPARE)

template<typename T, bool compare (const T&, const T&)>
class Set {

public:

//  CONSTRUCTORS

    Set () {};

    Set (std::initializer_list<T> lst) 
            : cap(lst.size()), sz(lst.size()), data(new T[lst.size()]) {
        std::copy(lst.begin(), lst.end(), data);
        std::sort(begin(), end(), compare);
    };

    Set (const Set &v) : cap (v.cap), sz (v.sz), data (new T[v.cap]) {
        std::copy (v.begin(), v.end(), data);
    };

    //  source: https://en.cppreference.com/w/cpp/language/copy_assignment
    //  (non copy-and-swap idiom)
    auto operator= (const Set &v) noexcept {
        // not a self-assignment
        if (this != &v) {
            // resource cannot be reused
            if (sz != v.size()) {
                //  in case of incorrect new 
                T* new_data = new T[v.size()];

                delete [] data;
                data = new_data;
                sz = v.size();
            }
            std::copy(v.begin(), v.end(), data);
        }
    };

    Set (Set &&v) noexcept {
        swap (v);
    }

    Set& operator= (Set &&v) noexcept {
        swap (v);
        v.sz = 0;
        v.cap = INIT_CAP;
        
        return *this;
    };

//  public?
    void swap (Set<T,compare> &v2) noexcept {
        std::swap (data, v2.data);
        std::swap (cap, v2.cap);
        std::swap (sz, v2.sz);
    }

//  DESTRUCTOR

    ~Set() { delete[] data; data = nullptr; cap = 0; sz = 0; }

    const T &operator[] (std::size_t i) const { 
        if (data == nullptr)
            throw std::runtime_error ("Access to uninitialized data");
        if (i > sz - 1)
            throw std::runtime_error ("Access beyond the bounds");
        return *(data + i); 
    }

    T &operator[] (std::size_t i) { 
        if (data == nullptr)
            throw std::runtime_error ("Access to uninitialized data");
        if (i > sz - 1)
            throw std::runtime_error ("Access beyond the bounds");
        return *(data + i); 
    }

    //  return enum 
    State state () const noexcept {
        if (sz == 0)    return EMPTY; 
        if (sz == cap)  return FULL;
        else            return PART;
    }

    //  add to selected position 

    //  incorrect add
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

