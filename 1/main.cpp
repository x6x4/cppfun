#include <iostream>
#include "libmatrix.h"


int main () {
    
    sparse_matrix::Sparse_mx mx = {};
    sparse_matrix::Sparse_mx& Mx = mx;
    sparse_matrix::Sparse_mx new_mx = {};
    sparse_matrix::Sparse_mx& New_mx = new_mx;

    try {
        mx = sparse_matrix::Input();
        
        //  create new matrix 
        new_mx = sparse_matrix::Create_modify(Mx);
        
        sparse_matrix::Output ("Source matrix", mx);
        sparse_matrix::Output ("Destination matrix", new_mx);

        //  free memory
        sparse_matrix::Erase (Mx);
        sparse_matrix::Erase (New_mx);
    }

    catch(const std::bad_alloc& ba) { // в случае ошибок выделения памяти
        std::cerr << "Not enough memory" << std::endl;
        //  free memory
        sparse_matrix::Erase (Mx);
        sparse_matrix::Erase (New_mx);
        return 1;
    }
    catch(const std::exception& e) { // в случае прочих исключений
        std::cerr << e.what() << std::endl;
        //  free memory
        sparse_matrix::Erase (Mx);
        sparse_matrix::Erase (New_mx);
        return 1;   
    }

    return 0;
}

