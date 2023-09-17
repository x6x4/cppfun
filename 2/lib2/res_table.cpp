#include "res_table.h"
#include <algorithm>
#include <vector>


//  CLASS Res_Table

int Res_Table::get_profit () noexcept {

    std::size_t sum = 0;

    for (auto r : this->vec) {
        sum += r.get_profit();
    }

    return sum;
}

Res &Res_Table::operator[](std::string name) {
    auto p = find (name);
    if (p.first) return *p.second;
    else throw std::runtime_error ("Name not found");
}

void Res_Table::rename (std::string old_name, std::string new_name) {
    Res &r = (*this)[old_name];
    r.set_name(new_name);
}

Res_Table Res_Table::operator* (int n) noexcept {

    for (Res & r : vec) {
        r = r * n;
    }

    return *this; 
}

/**
 * @brief       Add resource to table
 * @param       r  Resource 
 * @return      Changed table
 * @exception   std::runtime_error  Duplicate resource
 */
Res_Table &Res_Table::operator+= (Res r) {
    
    vec.inc_size();
    vec.resize();

    auto p = find (r.get_name());
    if (p.first)
        *p.second = *p.second + r;
    else {
        std::move (p.second, vec.end() - 1, p.second + 1);
        *p.second = r;
    }

    return *this;
}

/**
 * @brief       Delete resource from table
 * @param       r  Resource 
 * @exception   std::runtime_error  Empty table
 */

void Res_Table::del (Res r) {

/*    if (state() == EMPTY)
        throw std::runtime_error ("Empty table");

    auto p = (*this)[r.get_name()];*/

}