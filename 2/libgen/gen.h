#pragma once

#include <algorithm>
#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <utility>


void check_is (const std::istream& is);


namespace MY_CLASSES {

    enum State {
        EMPTY,
        PART,
        FULL
    };

    //  SET - ORDERED CONTAINER (KEY, COMPARE)

    template<typename T, bool compare (const T&, const T&)>
    class Set {

    public:

    //  CONSTRUCTORS

        Set () {};

        //  ordered from unordered
        Set (std::initializer_list<T> lst) : cap(lst.size()), sz(lst.size()) {

            T *buf = new T[lst.size()];

            try {
                std::copy(lst.begin(), lst.end(), buf);
                std::sort(buf, buf+sz, compare);
            }
            catch (...) {
                delete [] buf;
            }

            data = buf;
        };

        //  ordered from ordered
        Set (const Set &v) : cap (v.cap), sz (v.sz) {

            T *buf = new T[v.cap];

            try {
                std::copy (v.begin(), v.end(), buf);
            }
            catch (...) {
                delete [] buf;
            }

            data = buf;
        };

        //  copy & swap idiom
        Set& operator= (const Set &v) noexcept {

            Set buf(v);
            swap(buf);
            return *this;
        };

        Set (Set &&v) noexcept {
            swap (v);
        }

        Set& operator= (Set &&v) noexcept {

            Set buf(std::move(v));
            swap(buf);
            return *this;
        };


    //  DESTRUCTOR

        ~Set() { if (data) delete[] data; }


    //  GETTERS

        const T &operator[] (const T &s) const {

            auto p = find(s);
            if (p.first == 0) throw std::runtime_error ("Item not found");

            return *p.second;
        }

        T &operator[] (const T &s) { 

            auto p = find(s);
            if (p.first == 0) throw std::runtime_error ("Item not found");

            return *p.second;
        }


    //  BASIC OPERATIONS

        //  add: join if s already exists
        Set &operator+= (T elm) {

            if (full()) cap ? resize(cap*2) : resize(2);
            auto p = find (elm);
            if (p.first)
                throw std::runtime_error ("Element already exists");
            else {
                sz++;
                if (p.second) {
                    std::shift_right (p.second, end(), 1);
                    
                    *p.second = std::move(elm);
                }
            }   

            return *this;   
        }

        void del (const T &elm) {

            T &victim = (*this)[elm];

            std::shift_left (&victim, end(), 1);
            sz--;
        }


    //  GETTERS

        T* begin () const noexcept { return data; }
        T*   end () const noexcept { return data + sz; }
        const T* cbegin () const noexcept { return data; }
        const T*   cend () const noexcept { return data + sz; }

        std::size_t capacity() const noexcept { return cap; }
        std::size_t size()     const noexcept { return sz;  }
        bool empty()    const noexcept { return !sz; }
        bool full()     const noexcept { return sz == cap;  }

    //  DEVELOPERS-ONLY

    private:

        std::size_t cap = 0;
        std::size_t sz = 0;
        T* data = nullptr;

        void swap (Set<T,compare> &v2) noexcept {
            std::swap (data, v2.data);
            std::swap (cap, v2.cap);
            std::swap (sz, v2.sz);
        }

        //  return pair <is_found, ptr to pos>
        std::pair<bool, T*> find (const T& item) const noexcept {

            if (!data) return std::make_pair(0, nullptr);

            T* ptr = std::lower_bound(begin(), end(), item, compare);
            std::pair<bool, T*> p = {ptr != end() && compare(item, *ptr) == 0, ptr};

            return p;
        }

        void resize (std::size_t n) {

            cap = n;
            T* buf = new T[cap];
            try {
                std::move(begin(), end(), buf);
            }
            catch (...) {
                delete [] buf;
            }

            delete [] data;
            data = buf;
        }

    };

}