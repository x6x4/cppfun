#pragma once

#include "../libgen/gen.h"
#include <iostream>
#include <compare>


namespace MY_CLASSES {

    //  SIMPLE CLASS

    class Res {

    public:

    //  CONSTRUCTORS

        /**
        * @brief       Default resource constructor
        * @return      Created resource 
        */
        Res () {}; 

        /**
        * @brief       Initing resource constructor (all fields)
        * @param       name   Resource name
        * @param       cons   Resource consumption
        * @param       prod   Resource production
        * @param       price  Resource price
        * @return      Created resource
        * @exception   std::invalid argument  In cases of negative cons, prod or price.
        */
        explicit Res (const std::string &name, int cons, int prod, int price);
        
        /**
        * @brief       Initing Res constructor (name and price)
        * @param       name   Resource name
        * @param       price  Resource price
        * @return      Created resource
        * @exception   std::invalid argument  In case of negative price.
        */
        explicit Res (const std::string &name, int price);

    //  GETTERS

        /**
        * @brief       Get resource name
        * @return      Resource name
        */
        const std::string &get_name  () const noexcept { return name;  }

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
        Res &set_name  (const std::string &name) noexcept;

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
        * @return      Input stream ref
        * @exception   std::invalid argument  In cases of negative cons, prod or price.
        */
        friend std::istream& operator>> (std::istream& is, Res &r) { 
            
            is >> r.name;
            check_is(is);
            is >> r.cons;
            check_is(is);
            is >> r.prod;
            check_is(is);
            is >> r.price;

            r.check_fields(); 
            
            return is;
        }


    //  OTHER

        /**
        * @brief       Calculate resource week profit
        * @return      Resource week profit
        */
        int get_profit () const noexcept;

        /**
        * @brief       Increase turnover by the specified number of times
        * @param       n  Times 
        */
        Res operator* (int n) const noexcept;

    //  DEVELOPERS-ONLY

    private:

        std::string name  = "(default)";
        int cons  = 0;
        int prod  = 0;
        int price = 0;

        void check_fields () const;
    };


    /**
    * @brief       Output in format 
    <name> <consumption> <prodiction> <price>
    * @return      Output stream ref
    */
    std::ostream& operator<< (std::ostream& os, const Res &r) noexcept;

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
    * @return      true/false
    */
    bool operator< (const Res& r1, const Res& r2) noexcept;

    /**
    * @brief       Compare two resources by name
    * @param       r1  First resource
    * @param       r2  Second resource 
    * @return      less, greater or equivalent
    */
    std::weak_ordering operator<=> (const Res& r1, const Res& r2) noexcept;

    /**
    * @brief       Check if two resources are equal 
                    (have same fields values for all fields)
    * @param       r1  First resource
    * @param       r2  Second resource 
    * @return      true/false
    */
    bool equal (const Res &r1, const Res &r2);

}
