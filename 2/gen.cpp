#include <cstring>
#include <iostream>


void check_cin () {

    if (std::cin.eof()) 
        throw std::runtime_error ("Error reading number: EOF");

    else if (std::cin.bad())
        throw std::runtime_error(std::string("Error reading number:") + strerror(errno));

    //  non-blocking error: no exception
    else if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore();
        std::cout << "Invalid output: try again" << std::endl;
    }

    else return;
}