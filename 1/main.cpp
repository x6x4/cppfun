#include <iostream>
#include "libmatrix.h"

int main () {
    
    try {
        sparse_matrix::Sparse_mx mx = sparse_matrix::Input();
        
        //  create new matrix 
        sparse_matrix::Sparse_mx new_mx;

        sparse_matrix::Output ("Source matrix", mx);
        sparse_matrix::Output ("Destination matrix", new_mx);

        //  free memory
    }

    catch(const std::bad_alloc& ba) { // в случае ошибок выделения памяти
        std::cerr << "Not enough memory" << std::endl;
        //  free memory
        return 1;
    }
    catch(const std::exception& e) { // в случае прочих исключений
        std::cerr << e.what() << std::endl;
        //  free memory
        return 1;   
    }

    return 0;
}
