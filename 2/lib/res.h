#pragma once

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <iostream>
#include <compare>
#include <fmt/core.h>
#include <fmt/ranges.h>


enum state {
    EMPTY,
    PART,
    FULL
};


//  SIMPLE CLASS

class Res {

public:

    std::string name  = "(default)";
    int cons  = 0;
    int prod  = 0;
    int price = 0;

//  CONSTRUCTORS
    
    Res () {}; 

    /**
    * @brief       Initing Res constructor (all fields)
    * @param       name   Resource name
    * @param       cons   Resource consumption
    * @param       prod   Resource production
    * @param       price  Resource price
    * @return      Created resource
    * @exception   std::invalid argument  In cases of negative cons, prod or price.
    */
    explicit Res (std::string name, int cons, int prod, int price);
    
    /**
    * @brief       Initing Res constructor (name and price)
    * @param       name   Resource name
    * @param       price  Resource price
    * @return      Created resource
    * @exception   std::invalid argument  In case of negative price.
    */
    explicit Res (std::string name, int price);

//  GETTERS

    /**
    * @brief       Get resource name
    * @return      Resource name
    */
    auto get_name  () const noexcept { return name;  }

    /**
    * @brief       Get resource consumption
    * @return      Resource consumption
    */
    auto get_cons  () const noexcept { return cons;  }

    /**
    * @brief       Get resource production
    * @return      Resource production
    */
    auto get_prod  () const noexcept { return prod;  }

    /**
    * @brief       Get resource price
    * @return      Resource price
    */
    auto get_price () const noexcept { return price; }    

//  SETTERS

    /**
    * @brief       Set resource name
    * @param       name   Resource name
    * @return      Resource ref
    */
    Res &set_name  (std::string name) noexcept;

    /**
    * @brief       Set resource consumption
    * @param       cons   Resource consumption
    * @return      Resource ref
    * @exception   std::invalid_argument  In case of negative cons.
    */
    Res &set_cons  (int cons);

    /**
    * @brief       Set resource production
    * @param       prod   Resource production
    * @return      Resource ref
    * @exception   std::invalid_argument  In case of negative prod.
    */
    Res &set_prod  (int prod);

    /**
    * @brief       Set resource price
    * @param       prod   Resource price
    * @return      Resource ref
    * @exception   std::invalid_argument  In case of negative price.
    */
    Res &set_price (int price);

//  IO

    /**
    * @brief       Input in format 
    <name> <consumption> <prodiction> <price>
    * @exception   std::invalid argument  In cases of negative cons, prod or price.
    */
    friend std::istream& operator>> (std::istream& is, Res &r) { 
        is >> r.name >> r.cons >> r.prod >> r.price;
        r.check_fields();
        return is;
    }

    /**
    * @brief       Output in format 
    <name> <consumption> <prodiction> <price>
    */
    friend std::ostream& operator<< (std::ostream& os, const Res &r) noexcept { 
        os << r.name << ' ' << r.cons << ' '<< r.prod << ' ' << r.price;
        return os;
    }

//  OTHER

    /**
    * @brief       Calculate resource week profit
    * @return      Resource week profit
    */
    int get_profit () noexcept;

    /**
    * @brief       Increase turnover by the specified number of times
    * @param       n  Times 
    */
    void operator* (int n) noexcept;

//  DEVELOPERS-ONLY

private:
    void check_fields ();
};

/**
 * @brief       Join two same resources
 * @param       r1  First resource
 * @param       r2  Second resource 
 * @return      New united resource
 * @exception   std::logic_error  In cases of negative cons, prod or price.
 */
Res operator+ (const Res &r1, const Res &r2);
/**
 * @brief       Compare two resources by name
 * @param       r1  First resource
 * @param       r2  Second resource 
 * @return      less, greater or equivalent
 */
//std::weak_ordering operator<=> (const Res& r1, const Res& r2) noexcept;


//  VECTOR

template<typename T> 
class Vector {

public:

    int check () noexcept {
        if (sz == 0)    return EMPTY; 
        if (sz == cap)  return FULL;
        else            return PART;
    }

    Vector () {};
    Vector (std::initializer_list<T> lst) 
            : data(new T[lst.size()]), cap(lst.size()), sz(lst.size()) {
        std::copy(lst.begin(), lst.end(), data);
    };
    Vector (Vector &v) {
        data = new T[v.cap];
        std::copy(v.begin(), v.end(), data);
    };


    void resize () {
        if (check() == FULL) {
            T* new_data = new T[cap];
            std::move (begin(), end(), new_data);
            delete[] data;
        }
    }

    ~Vector() { delete[] data; }

    T* begin () { return data; }
    T*   end () { return data + this->sz; }

    void swap (Vector<T> &v2) {
        std::swap (data, v2.data);
        std::swap (cap, v2.cap);
        std::swap (sz, v2.sz);
        std::cout << "Vec" << std::endl;
    }

//  DEVELOPERS-ONLY

private:
    T* data;
    std::size_t cap = 0;
    std::size_t sz  = 0; 
};


//  COMPLEX CLASS

class Res_Table {

public:

//  CONSTRUCTORS

    Res_Table () {};  //  default

    //  init

    Res_Table (std::initializer_list<Res> data) : vec(data) {};

    //  copy

    Res_Table (Res_Table &t) : vec (t.vec) {};

    auto operator= (Res_Table &t) 
    {
        swap (t);
        std::cout << "vec";
        return *this;
    }

    void swap (Res_Table &t) {
        vec.Vector<Res>::swap(t.vec);
    }

    //  move

    //Res_Table (const Res_Table &&t);
    //Res_Table &operator= (const Res_Table &&t);

//  DESTRUCTOR

    ~Res_Table () = default;

//  IO

    friend std::ostream& operator<< (std::ostream& os, Res_Table &t) { 
        for (auto r : t.vec) {
            os << r << std::endl;
        }
        return os;
    }

//  OTHER

    //  add resource
    auto operator+= (Res r);

    //  get resource by name
    auto operator[] (std::string name);

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

