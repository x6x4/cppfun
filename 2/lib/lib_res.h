#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include <iostream>


enum state {
    EMPTY,
    PART,
    FULL
};


class Res {

public:

//  CONSTRUCTORS
    
    Res () {};  //  default

    //  init

    Res (std::string name, std::size_t cons, std::size_t prod, std::size_t price) : 
                     name(name), cons(cons), prod(prod), price(price) {};
    Res (std::string name, std::size_t price) : name(name), price(price) {};

//  GETTERS


//  SETTERS


//  IO

    /*friend std::istream& operator>> (std::istream& is, const Res &r) { 
        is >> r.name >> r.cons >> r.price >> r.prod;
        return is;
    }*/

    friend std::ostream& operator<< (std::ostream& os, const Res &r) { 
        os << r.name << ' ' << r.cons << ' '<< r.prod << ' ' << r.price;
        return os;
    }


//  OTHER

    //  join same resources
    Res operator+ (Res r);

    //  compare two resources by name
    bool operator< (const Res& r) const { return name < r.name; }

    //  calculate resource week profit
    std::size_t get_profit ();

    //  multiply resource turnover 
    void operator* (std::size_t n);


//  DEVELOPERS-ONLY

//  gains access to fields
friend class Res_Table;

private:
    //  fields
    std::string name  = "(default)";
    std::size_t cons  = 0;
    std::size_t prod  = 0;
    std::size_t price = 0;

};

template<typename T> 
class Vector {

public:

    int check () noexcept {
        if (sz == 0)    return EMPTY; 
        if (sz == cap)  return FULL;
        else            return PART;
    }

    Vector () {};
    Vector (T* data, std::size_t sz) : data(data), cap(sz), sz(sz) {};

    const T* begin () { return data; }
    const T*   end () { return data + this->sz; }


//  DEVELOPERS-ONLY

private:
    T* data;
    std::size_t cap = 0;
    std::size_t sz  = 0; 
};


class Res_Table {

public:

//  CONSTRUCTORS

    Res_Table () {};  //  default

    //  init

    Res_Table (Res *data, std::size_t sz) : vec(data, sz) {};

    //  copy


    //  move


//  DESTRUCTOR



//  IO

    friend std::ostream& operator<< (std::ostream& os, Res_Table &t) { 
        for (auto r : t.vec) {
            os << r << std::endl;
        }
        return os;
    }

//  OTHER

    //  add resource
    void operator+= (Res r);

    //  get resource by name
    Res &operator[] (std::string name);

    //  set resource name
    void set (std::string old_name, std::string new_name);

    //  check table state
    int check () noexcept;

    //  delete resource
    void del (Res r);

    //  calculate total week profit for all resources
    std::size_t get_profit ();

    //  multiply turnover for all resources
    void operator* (std::size_t n);


//  DEVELOPERS-ONLY

private:
    //  fields
    Vector<Res> vec;

    //  methods
    std::pair<bool, Res> find (std::string name) noexcept;
};

