#include <iostream>
#include "libmatrix.h"


int main () {
    
    sparse_matrix::Sparse_mx mx = {};
    sparse_matrix::Sparse_mx new_mx = {};

    try {
        mx = sparse_matrix::Input();
        
        new_mx = sparse_matrix::Create_modify(mx);
        
        sparse_matrix::Output ("Source matrix", mx);
        sparse_matrix::Output ("Destination matrix", new_mx);
    }

    catch(const std::bad_alloc& ba) { // в случае ошибок выделения памяти
        std::cerr << "Not enough memory" << std::endl;
        return 1;
    }
    catch(const std::exception& e) { // в случае прочих исключений
        std::cerr << e.what() << std::endl;
        return 1;   
    }

    return 0;
}

