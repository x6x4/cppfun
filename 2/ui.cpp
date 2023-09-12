#include <iostream>
#include "lib/lib_res.h"


int main (void) {

    Res res_a = {"A", 50, 70, 10};
    Res res_b = {"A", 60, 110, 20};
    Res res_c = res_a + res_b;

    std::cout << res_c << std::endl;

    try {

    }

    catch(const std::bad_alloc& ba) { 
        std::cerr << "Not enough memory" << std::endl;
        return 1;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;   
    }

    return 0;
}

