#include "res_table.h"
#include <algorithm>
#include <vector>

namespace MY_CLASSES {

    //  CLASS Res_Table

    int Res_Table::get_profit () const noexcept {

        std::size_t sum = 0;

        for (auto r : set) 
            sum += r.get_profit();

        return sum;
    }

    Res &Res_Table::operator[](const std::string &name) {
        return set[Res(name, 0)];
    }

    const Res &Res_Table::operator[](const std::string &name) const {
        return set[Res(name, 0)];
    }

    void Res_Table::rename (const std::string &old_name, const std::string &new_name) {
        Res &r = (*this)[old_name];
        r.set_name(new_name);
    }

    Res_Table Res_Table::operator* (int n) noexcept {

        Res_Table t {*this};

        for (Res & r : t.set) 
            r = r * n;

        return t; 
    }

    std::ostream& operator<< (std::ostream& os, Res_Table &t) noexcept { 
        if (t.size() == 0) 
            os << "Empty table";
        else {
            for (auto r : t.set) {
                os << r << std::endl;
            }
        }
        
        return os;
    }

    Res_Table &Res_Table::operator+= (Res r) {
        set+=r;
        return *this;
    }

    void Res_Table::del (const std::string &name) {
        set.del(Res(name, 0));
    }

}
