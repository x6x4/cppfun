#pragma once

#include "../libgen/gen.h"
#include "../lib1/res.h"
#include <algorithm>
#include <utility>


//  COMPLEX CLASS

class Res_Table {

public:

    Vector<Res> vec = {};

//  CONSTRUCTORS

    /**
    * @brief       Default resource table constructor
    * @return      Created resource table
    */
    Res_Table () {};

    /**
    * @brief       Initing resource table constructor
    * @param       data   Initializer list of resources
    * @return      Created resource table
    */
    Res_Table (const std::initializer_list<Res> &data) noexcept : vec(data) {};

    /**
    * @brief       Copying resource table constructor
    * @param       t   Table to copy from
    * @return      Copy of t
    */
    Res_Table (const Res_Table &t) noexcept : vec (t.vec) {};
    /**
    * @brief       Copy assignment operator
    * @param       t   Table to copy from
    * @return      Copy of t
    */
    auto operator= (const Res_Table &t) noexcept {};

    /**
    * @brief       Moving resource table constructor
    * @param       t   Table to move 
    * @return      Copy of t
    */
    Res_Table (Res_Table &&t) noexcept { 
        swap (t);
        t.~Res_Table();
    }
    /**
    * @brief       Move assignment operator
    * @param       t   Table to move
    * @return      Copy of t
    */
    auto operator= (Res_Table &&t) noexcept {};
        
//  DESTRUCTOR

    /**
    * @brief       Default resource table destructor
    */
    ~Res_Table() {};

//  GETTERS

    /**
    * @brief   Check table state
    * @return  Table state as integer
    */   
    int state () noexcept { return vec.state(); }

    /**
    * @brief       Get current table size.
    * @return      Amount of resources in table
    */
    std::size_t size() noexcept { return vec.size(); }
    /**
    * @brief       Get week profit for all resources.
    * @return      Total week profit 
    */
    int get_profit () noexcept;

    /**
    * @brief       Find resource by name
    * @param       name  Resource name
    * @return      Resource ref
    * @exception   std::runtime_error Name not found
    */
    Res &operator[] (std::string name);

//  SETTERS

    /**
    * @brief       Set resource name
    * @param       old_name  Old resource name
    * @param       new_name  New resource name
    */
    void rename (std::string old_name, std::string new_name);

    /**
    * @brief       Increase turnout for all resources 
                   by the specified number of times
    * @param       n  Times 
    * @return      Temporary value - copy of changed table
    */
    Res_Table operator* (int n) noexcept;

//  IO

    friend std::istream& operator>> (std::istream& is, Res_Table &t) { 
        Res r;
        while (is.good()) {
            is >> r;
            //t += r;
        }
        return is;
    }

    friend std::ostream& operator<< (std::ostream& os, Res_Table &t) noexcept { 
        for (auto r : t.vec) {
            os << r << std::endl;
        }
        return os;
    }

//  OTHER

    //  add resource
    Res_Table &operator+= (Res r);

    //  delete resource
    void del (Res r);

//  DEVELOPERS-ONLY

private:

    void swap (Res_Table &t) {
        vec.Vector<Res>::swap(t.vec);
    }
    
    /**
    * @brief       Find resource by name
    * @param       name  Resource name
    * @return      Pair <is_found, resouce ref>
    */
    std::pair<bool, Res&> find (std::string name) noexcept {
        auto r = std::lower_bound(vec.begin(), vec.end(), name, operator<=>);
        std::pair<bool, Res&> p = std::make_pair(r != std::end(vec), r);
        return p;
    };
};

