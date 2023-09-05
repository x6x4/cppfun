#pragma once

//  Library for operations with numbers

//  input-output stream library
#include <iostream>
//  min-max values library
#include <limits>
//  string library
#include <cstring>


#define TYPE_MAX std::numeric_limits<T>::max() 
#define TYPE_MIN std::numeric_limits<T>::min()
#define BETWEEN(min, num, max) (num >= min && num <= max)

namespace Numbers {

    //  Template function for generic number type. T is abstract type, nothing more.
    template <typename T> 

    //  default values are TYPE_MIN and TYPE_MAX
    T get_num (T min = TYPE_MIN, T max = TYPE_MAX) {

        T num;

        while (true) {
            std::cin >> num;
            
            if (std::cin.eof()) 
                throw std::runtime_error ("Error reading number: EOF");

            else if (std::cin.bad())
                throw std::runtime_error(std::string("Error reading number:") + strerror(errno));

            //  non-blocking error 
            else if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(TYPE_MAX, '\n');
                std::cout << "Invalid output: try again" << std::endl;
            }
            
            else if (BETWEEN(min, num, max))
                return num;

            else continue;
        }
    }
}