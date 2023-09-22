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
        Set (std::initializer_list<T> lst) 
                : cap(lst.size()), sz(lst.size()), data(new T[lst.size()]) {
            std::copy(lst.begin(), lst.end(), data);
            std::sort(begin(), end(), compare);
        };

        //  ordered from ordered
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


    //  DESTRUCTOR

        ~Set() { delete[] data; data = nullptr; cap = 0; sz = 0; }


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

        //  return enum 
        State state () const noexcept {
            if (sz == 0)    return EMPTY; 
            if (sz == cap)  return FULL;
            else            return PART;
        }


    //  BASIC OPERATIONS

        //  add: join if s already exists
        Set &operator+= (const T &s) {

            if (state() == FULL) resize(capacity()*2);

            auto p = find (s);
            if (p.first)
                *p.second = *p.second + s;
            else {
                sz++;
                if (p.second) {
                    std::shift_right (p.second, end(), 1);
                    *p.second = s;
                }
            }   

            return *this;   
        }

        void del (const T &s) {

            if (state() == EMPTY)
                throw std::logic_error ("Empty");

            T &victim = (*this)[s];

            std::shift_left (&victim, end(), 1);
            sz--;
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

        void swap (Set<T,compare> &v2) noexcept {
            std::swap (data, v2.data);
            std::swap (cap, v2.cap);
            std::swap (sz, v2.sz);
        }

        //  return pair <is_found, ptr to pos>
        std::pair<bool, T*> find (const T& item) const noexcept {

            T* ptr = std::lower_bound(begin(), end(), item, compare);
            std::pair<bool, T*> p = {ptr != end() && compare(item, *ptr) == 0, ptr};

            return p;
        }

        void resize (std::size_t n) {

            if (n < sz) throw std::runtime_error ("Shrinking is not allowed.");
            cap = n;
            T* new_data = new T[cap];

            std::move(begin(), end(), new_data);
            delete [] data;
            data = new_data;
        }

    };

}