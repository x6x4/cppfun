//  Library for operations with matrices

#include "libmatrix.h"
#include "libnums.h"
#include <iostream>
#include <utility>


namespace sparse_matrix {

    Sparse_mx Input() {
        
        Sparse_mx mx;

        try {
            std::cout << "Enter number of lines: ";
            std::size_t mx_height = Numbers::get_num<int>();

            std::cout << "Enter number of items in line: ";
            mx.width = Numbers::get_num<int>();

            for (std::size_t i = 0; i < mx_height; i++) {
                Sparse_line ln = sparse_line::Input (mx.width);
                mx.m.push_back (ln);
            }

            std::cout << std::endl;
        }

        catch (...) { 
            Erase (mx); 
            throw; 
        }

        return mx;
    }

    void Output (const char *msg, const Sparse_mx mx) {

        std::cout << msg << ":\n";

        for (const auto& ln : mx.m) {

            std::size_t cnt = 0;

            for (const auto& val : ln) {
                for (; val.first > cnt; cnt++) 
                    std::cout << 0 << " ";
                cnt = val.first + 1;
                
                std::cout << val.second << " ";
            }
            for (; mx.width > cnt; cnt++) 
                    std::cout << 0 << " ";

            std::cout << std::endl;
        }

        std::cout << std::endl;
    }

    void Erase (Sparse_mx& matrix) {
        
    }

}

namespace sparse_line {

    Sparse_line Input (std::size_t ln_width) {

        Sparse_line ln;
        std::cout << "Enter items in line: ";

        for (std::size_t i = 0; i < ln_width; i++) {

            int val = Numbers::get_num<int>();
    
            if (val) {
                List_pair pair = {i, val};
                ln.push_back (pair);
            }
        }

        return ln;
    }
}
