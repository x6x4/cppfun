#include <iostream>
#include "lib/res.h"


int main (void) {

    Res res_a {"A", 10, 20, 30};
    Res res_b {"B", 40};
    Res res_c {"A", 80, 90, 50};

    try {

    Res res_d = res_a + res_c;

    std::cout << res_d << std::endl;
    //std::cout << res_b << std::endl;

    Res_Table t {res_a, res_b, res_c};
    Res_Table t2 = t;

    std::cout << t2 << std::endl;

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

