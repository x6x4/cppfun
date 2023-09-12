#include "lib_res.h"
#include <algorithm>
#include <stdexcept>

//  Class Res_Table

/**
 * @brief   Check table state
 * @return  Table state as integer
 */

int Res_Table::check () noexcept {
    if (vec.sz == 0)        return EMPTY; 
    if (vec.sz == vec.cap)  return FULL;
    else                    return PART;
}

/**
 * @brief       Find resource by name
 * @param       name  Resource name
 * @return      Program exit status
 * @exception   std::runtime_error Name not found
 */

Res &Res_Table::operator[](std::string name) {
    auto range = std::equal_range(vec.begin(), vec.end(), name);
    if (range.first == range.second) {
        throw std::runtime_error ("Name not found");
    }
    return this->vec.data[range.first];
}


void del (Res r) {

}

