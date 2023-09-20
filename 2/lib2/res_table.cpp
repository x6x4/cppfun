#include "res_table.h"
#include <algorithm>
#include <vector>


//  CLASS Res_Table

int Res_Table::get_profit () const noexcept {

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

const Res &Res_Table::operator[](std::string name) const {
    auto p = find (name);
    if (p.first) return *p.second;
    else throw std::runtime_error ("Name not found");
}

void Res_Table::rename (std::string old_name, std::string new_name) {
    Res &r = (*this)[old_name];
    r.set_name(new_name);
}

Res_Table Res_Table::operator* (int n) noexcept {

    Res_Table t {*this};

    for (Res & r : t.vec) {
        r = r * n;
    }

    return t; 
}

std::ostream& operator<< (std::ostream& os, Res_Table &t) noexcept { 
    if (t.size() == 0) 
        os << "Empty table";
    else {
        for (auto r : t.vec) {
            os << r << std::endl;
        }
    }
    
    return os;
}

Res_Table &Res_Table::operator+= (Res r) {

    if (state() == FULL) vec.resize(vec.capacity()*2);

    auto p = find (r.get_name());
    if (p.first)
        *p.second = *p.second + r;
    else {
        vec.inc_size();
        if (p.second) {
            std::shift_right (p.second, vec.end(), 1);
            *p.second = r;
        }
    }

    return *this;
}

void Res_Table::del (std::string name) {

    if (state() == EMPTY)
        throw std::logic_error ("Empty table");

    Res &victim = (*this)[name];

    std::shift_left (&victim, vec.end(), 1);
    vec.dec_size();
}