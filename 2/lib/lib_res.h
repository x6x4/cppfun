
#include <cstddef>
#pragma once

#include <string>
#include <vector>
#include <iostream>


class Res {
public:
    std::string name  = "";
    std::size_t cons  = 0;
    std::size_t prod  = 0;
    std::size_t price = 0;

    //  constructors

    //  io

    /*friend std::istream& operator>> (std::istream& is, const Res &r) { 
        is >> r.name >> r.cons >> r.price >> r.prod;
        return is;
    }*/

    friend std::ostream& operator<< (std::ostream& os, const Res &r) { 
        os << r.name << ' ' << r.cons << ' '<< r.prod << ' ' << r.price;
        return os;
    }

    //  join same resources
    Res operator+ (Res r);

    //  compare two resources by name
    bool operator< (const Res& r) const { return name < r.name; }

    //  calculate resource week profit
    std::size_t get_profit ();

    //  multiply resource turnover 
    void operator* (std::size_t n);
};

template<typename T> 
class Vector {
public:
    T* data;
    std::size_t cap = 0;
    std::size_t sz  = 0; 

    const T* begin () { return data; }
    const T*   end () { return data + this->sz - 1; }
    
};


class Res_Table {

    std::pair<bool, Res> find (std::string name) noexcept;

public:
    Vector<Res> vec;

    //  constructors



    //  io

    friend std::ostream& operator<< (std::ostream& os, Res_Table &t) { 
        for (auto r : t.vec) {
            os << r;
        }
        return os;
    }

    //  add resource
    void operator+= (Res r);

    //  get resource by name
    Res &operator[] (std::string name);

    //  set resource name
    void set (std::string old_name, std::string new_name);

    enum state {
        EMPTY,
        PART,
        FULL
    };

    //  check table state
    int check () noexcept;

    //  delete resource
    void del (Res r);

    //  calculate total week profit for all resources
    std::size_t get_profit ();

    //  multiply turnover for all resources
    void operator* (std::size_t n);
};

