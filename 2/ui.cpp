#include <iostream>
#include "lib1/res.h"
#include "lib2/res_table.h"


int main (void) {

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

