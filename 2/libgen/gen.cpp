#include <cstring>
#include <iostream>
#include "gen.h"


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