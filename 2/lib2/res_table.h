#pragma once

#include "../libgen/gen.h"
#include "../lib1/res.h"
//#include <algorithm>
//#include <cstdlib>
//#include <utility>
//#include <compare>


namespace MY_CLASSES {

    //  COMPLEX CLASS

    class Res_Table {

    public:

        Set<Res, operator<(const Res &r1, const Res &r2)> set = {};

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
        Res_Table (const std::initializer_list<Res> &data) noexcept : set(data) {};

        /**
        * @brief       Copying resource table constructor
        * @param       t   Table to copy from
        * @return      Copy of t
        */
        Res_Table (const Res_Table &t) noexcept : set (t.set) {};
        /**
        * @brief       Copy assignment operator
        * @param       t   Table to copy from
        * @return      Copy of t
        */
        auto operator= (const Res_Table &t) noexcept {
            set = t.set;
        };

        /**
        * @brief       Moving resource table constructor
        * @param       t   Table to move 
        * @return      Copy of t
        */
        explicit Res_Table (Res_Table &&t) noexcept : set (std::move(t.set)) {};

        /**
        * @brief       Move assignment operator
        * @param       t   Table to move
        * @return      Ref to itself
        */
        //  source: https://en.cppreference.com/w/cpp/language/move_assignment
        Res_Table& operator= (Res_Table &&t) noexcept {
            set = std::move(t.set);
            return *this;
        };
            
    //  DESTRUCTOR

        /**
        * @brief       Default resource table destructor
        */
        ~Res_Table() {};

    //  GETTERS

        /**
        * @brief   Check table state
        * @return  Table state enum
        */   
        State state () const noexcept { return set.state(); }

        /**
        * @brief       Get current table size.
        * @return      Amount of resources in table
        */
        std::size_t size() const noexcept { return set.size(); }
        /**
        * @brief       Get week profit for all resources.
        * @return      Total week profit 
        */
        int get_profit () const noexcept;

        /**
        * @brief       Find resource by name
        * @param       name  Resource name
        * @return      Resource ref
        * @exception   std::runtime_error Name not found
        */
        Res &operator[] (const std::string &name);
        const Res &operator[] (const std::string &name) const;

    //  SETTERS

        /**
        * @brief       Set resource name
        * @param       old_name  Old resource name
        * @param       new_name  New resource name
        */
        void rename (const std::string &old_name, const std::string &new_name);

        /**
        * @brief       Increase turnout for all resources 
                       by the specified number of times
        * @param       n  Times 
        * @return      Temporary value - copy of changed table
        */
        Res_Table operator* (int n) noexcept;

    //  IO

        /**
        * @brief       Input table
        * @return      Input stream ref
        */
        friend std::istream& operator>> (std::istream& is, Res_Table &t) { 
            Res r;
            while (is.good()) {
                is >> r;
                t += r;
            }
            return is;
        }


    //  OTHER

        /**
        * @brief       Add resource to table. 
                       IF DUPLICATE: adds new resource to current.:  
        * @param       r  Resource 
        * @return      Changed table
        */
        Res_Table &operator+= (Res r);

        /**
        * @brief       Delete resource from table by name
        * @param       name  Resource name
        * @exception   std::runtime_error  Empty table
        */
        void del (const std::string &name);


    //  DEVELOPERS-ONLY

    private:
        
        /**
        * @brief       Find matching resource position by name 
        * @param       name  Resource name
        * @return      Pair <is_found, resouce ref>
        */
        std::pair<bool, Res*> find (std::string name) const noexcept {
            auto r = std::lower_bound(set.begin(), set.end(), Res(name, 0), operator<);
            std::pair<bool, Res*> p = {r != std::end(set) && (r->get_name() == name), r};
            return p;
        };

    };


    /**
    * @brief       Output table
    * @return      Output stream ref
    */
    std::ostream& operator<< (std::ostream& os, Res_Table &t) noexcept; 

}
