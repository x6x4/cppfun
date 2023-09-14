#include <iostream>
#include "lib/lib_res.h"


int main (void) {

    Res res_a {"A", 10, 20, 30};
    Res res_b {"B", 40};

    std::cout << res_a << std::endl;
    std::cout << res_b << std::endl;

    Res res[] = {res_a, res_b};

    Res_Table t {res, 2};

    std::cout << t << std::endl;

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

