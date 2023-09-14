#include "lib_res.h"
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <vector>


//  CLASS Res

/**
 * @brief       Join two same resources
 * @param       r  Second resource 
 * @exception   std::runtime_error  Duplicate resource
                std::runtime_error  Full table
 */
Res Res::operator+ (Res r) {

    this->cons = this->cons + r.cons;
    this->prod = this->prod + r.prod;
    this->price = (this->price < r.price) ? this->price : r.price;

    return *this;
}

/**
 * @brief       Get week profit.
 */
std::size_t Res::get_profit () {
    return (this->prod - this->cons)*this->price*7;
}

/**
 * @brief       Increase turnout by the specified number of times
 * @param       n  Times 
 */
void Res::operator* (std::size_t n) {

    this->cons = this->cons * n;
    this->prod = this->prod * n;
}


//  CLASS Res_Table


/**
 * @brief       Add resource to table
 * @param       r  Resource 
 * @exception   std::runtime_error  Duplicate resource
                std::runtime_error  Full table
 */
void Res_Table::operator+= (Res r) {

    if (check() == FULL)
        throw std::runtime_error ("Full table");

    auto p = find (r.name);
    if (p.first)
        throw std::runtime_error ("Duplicate resource");
    else 
    ;
}

/**
 * @brief   Check table state
 * @return  Table state as integer
 */

int Res_Table::check () noexcept {
    return vec.check();
}

/**
 * @brief       Find resource by name
 * @param       name  Resource name
 * @return      Pair <is_found, matching place>
 */
std::pair<bool, Res> Res_Table::find (std::string name) noexcept {
    /*auto range = std::equal_range(vec.begin(), vec.end(), name);
    
    std::pair<bool, Res> p;
    p.second = *range.first;
    p.first = !(range.first == range.second);

    return p;*/
    return {};
}

/**
 * @brief       Find resource by name
 * @param       name  Resource name
 * @return      Resource ref
 * @exception   std::runtime_error Name not found
 */

Res &Res_Table::operator[](std::string name) {
    auto p = find (name);
    if (p.first)
        return p.second;
    else 
        throw std::runtime_error ("Name not found");
}

/**
 * @brief       Set resource name
 * @param       old_name  Old resource name
 * @param       new_name  New resource name
 */

void Res_Table::set (std::string old_name, std::string new_name) {
    auto p = (*this)[old_name];
    p.name = new_name;
}

/**
 * @brief       Delete resource from table
 * @param       r  Resource 
 * @exception   std::runtime_error  Empty table
 */

void Res_Table::del (Res r) {

    if (check() == EMPTY)
        throw std::runtime_error ("Empty table");

    auto p = (*this)[r.name];

}


/**
 * @brief       Get week profit for all resources.
 */

std::size_t Res_Table::get_profit () {

    std::size_t sum = 0;

    for (auto r : this->vec) {
        sum += r.get_profit();
    }

    return sum;
}

/**
 * @brief       Increase turnout for all resources 
                by the specified number of times
 * @param       n  Times 
 */
void Res_Table::operator* (std::size_t n) {
    for (auto r : this->vec) {
        r * n;
    }
}