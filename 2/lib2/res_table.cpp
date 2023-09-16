#include "res_table.h"


//  CLASS Res_Table


/**
 * @brief       Add resource to table
 * @param       r  Resource 
 * @exception   std::runtime_error  Duplicate resource
 */
Res_Table &Res_Table::operator+= (Res r) {

    vec.resize();

    auto p = find (r.name);
    if (p.first)
        throw std::runtime_error ("Duplicate resource");
    else 
        ;

    return *this;
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
}

/**
 * @brief       Find resource by name
 * @param       name  Resource name
 * @return      Resource ref
 * @exception   std::runtime_error Name not found
 */

auto Res_Table::operator[](std::string name) {
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

int Res_Table::get_profit () {

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
Res_Table Res_Table::operator* (int n) {

    for (auto r : this->vec) {
        r = r * n;
    }

    return *this; 
}
