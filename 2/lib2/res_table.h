#pragma once

#include "../lib1/res.h"

enum state {
    EMPTY,
    PART,
    FULL
};

//  VECTOR

template<typename T> 
class Vector {

public:

    T* data = nullptr;
    std::size_t cap = 0;
    std::size_t sz  = 0; 

//  CONSTRUCTORS

    Vector () {};

    //  copy

    Vector (std::initializer_list<T> lst) 
            : data(new T[lst.size()]), cap(lst.size()), sz(lst.size()) {
        std::copy(lst.begin(), lst.end(), data);
    };

    Vector (const Vector &v) : data (new T[v.cap]), cap (v.cap), sz (v.sz) {
        std::copy (v.begin(), v.end(), data);
    };

    //  move

    Vector (Vector &&v) noexcept {
        swap (v);
        ~v;
    }

    void swap (Vector<T> &v2) {
        std::swap (data, v2.data);
        std::swap (cap, v2.cap);
        std::swap (sz, v2.sz);
    }

//  DESTRUCTOR

    ~Vector() { delete[] data; data = nullptr; cap = 0; sz = 0; }

    int check () noexcept {
        if (sz == 0)    return EMPTY; 
        if (sz == cap)  return FULL;
        else            return PART;
    }

    void resize () {
        if (check() == FULL) {
            T* new_data = new T[cap];
            std::move (begin(), end(), new_data);
            delete[] data;
        }
    }



    T* begin () const { return data; }
    T*   end () const { return data + this->sz; }



//  DEVELOPERS-ONLY

private:

};


//  COMPLEX CLASS

class Res_Table {

public:

    Vector<Res> vec;

//  CONSTRUCTORS

    Res_Table () {};
    Res_Table (std::initializer_list<Res> data) : vec(data) {};

    //  copy

    Res_Table (const Res_Table &t) : vec (t.vec) {};
    auto operator= (const Res_Table &t) {};

    //  move

    Res_Table (Res_Table &&t) noexcept { 
        swap (t); 
        t.vec.~Vector<Res>(); 
    }

    auto operator= (Res_Table &&t) noexcept {};
        
    void swap (Res_Table &t) {
        vec.Vector<Res>::swap(t.vec);
    }

//  DESTRUCTOR

    ~Res_Table() {};

//  IO

    friend std::istream& operator>> (std::istream& is, Res_Table &t) { 
        Res r;
        while (is.good()) {
            is >> r;
            //t += r;
        }
        return is;
    }

    friend std::ostream& operator<< (std::ostream& os, Res_Table &t) { 
        for (auto r : t.vec) {
            os << r << std::endl;
        }
        return os;
    }

//  OTHER

    //  add resource
    Res_Table &operator+= (Res r);

    //  get resource by name
    auto operator[] (std::string name);

    //  set resource name
    void set (std::string old_name, std::string new_name);

    //  check table state
    int check () noexcept;

    //  delete resource
    void del (Res r);

    //  calculate total week profit for all resources
    int get_profit ();

    //  multiply turnover for all resources
    Res_Table operator* (int n);


//  DEVELOPERS-ONLY

private:


    //  methods
    std::pair<bool, Res> find (std::string name) noexcept;
};

