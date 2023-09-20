#include <cstring>
#include <iostream>
#include "gen.h"

void SubMenu::dialog () {

    std::cout << *this;

    

}

std::ostream &operator<<(std::ostream &os, SubMenu s) {

    os << s.name << "\n";

    for (auto e : s.submenus) {
        os << e;
    }
    return os;
}



void check_is (const std::istream& is) {

    if (is.eof()) 
        throw std::runtime_error ("Error reading data: EOF");

    else if (is.bad())
        throw std::runtime_error(std::string("Error reading data:") + strerror(errno));

    //  non-blocking error: no exception
    else if (is.fail()) {
        //is.clear();
        //is.ignore();
        std::cout << "Invalid output: try again" << std::endl;
    }

    else return;
}