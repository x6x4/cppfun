#pragma once

#include <iostream>
#include <limits>
#include <cstring> 


//----------------  Library for operations with numbers  ----------------//


template <typename T>
constexpr T MAX() { return std::numeric_limits<T>::max(); }
template <typename T>
constexpr T MIN() { return std::numeric_limits<T>::min(); }

template <typename T>
constexpr bool BETWEEN(T min, T num, T max) { return num >= min && num <= max; }


namespace Numbers {

    template <typename T> 

    T get_num (T min = MIN<T>(), T max = MAX<T>()) {

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
                std::cin.ignore(MAX<T>(), '\n');
                std::cout << "Invalid output: try again" << std::endl;
            }
            
            else if (BETWEEN<T>(min, num, max))
                return num;
        }
    }
}
