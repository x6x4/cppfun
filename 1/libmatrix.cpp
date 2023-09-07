
//----------------  Library for operations with sparse matrices  ----------------//

#include "libmatrix.h"
#include "libnums.h"
#include <iostream>


namespace sparse_matrix {

    Sparse_mx Input() {
        
        Sparse_mx mx;

        try {
            std::cout << "Enter number of lines: ";
            mx.height = Numbers::get_num<int>();

            std::cout << "Enter number of items in line: ";
            mx.width = Numbers::get_num<int>();

            for (std::size_t i = 0; i < mx.height; i++) {
                Sparse_line *ln = sparse_line::Input (mx.width);
                if (ln) mx.m.push_back ({i, ln});
            }

            std::cout << std::endl;
        }

        catch (...) { 
            Erase (mx); 
            throw; 
        }

        return mx;
    }

    void Output (const char *msg, const Sparse_mx mx) noexcept {

        std::cout << msg << ":\n";

        std::size_t line_cnt = 0;

        for (const auto& ln_pair : mx.m) {

            for (; ln_pair.first > line_cnt; line_cnt++) 
                sparse_line::Output_empty(mx.width);

            line_cnt = ln_pair.first + 1;

            sparse_line::Output (ln_pair.second, mx.width);
        }

        for (; mx.height > line_cnt; line_cnt++) 
            sparse_line::Output_empty(mx.width);

        std::cout << std::endl;
    }

    Sparse_mx Create_modify (Sparse_mx &mx) {

        Sparse_mx new_mx = {};

        try {

            std::size_t line_num = 0;

            for (const auto& ln : mx.m) {

                auto new_ln = new Sparse_line {};

                int nonzero_sgn = 0;

                for (const auto& val : *ln.second) {

                    if (!nonzero_sgn) {
                        
                        if (val.second > 0) 
                            nonzero_sgn = 1; 
                        if (val.second < 0) 
                            nonzero_sgn = -1; 
                    }

                    if (nonzero_sgn == 1 && val.second > 0)
                        new_ln->push_back(val);

                    else if (nonzero_sgn == -1 && val.second < 0)
                        new_ln->push_back(val);

                    else {
                        new_ln->push_back(val);
                        break;
                    }
                }

                if (new_ln && new_ln->size() == 0) {
                    delete (new_ln);
                    new_ln = nullptr;
                }

                if (new_ln) 
                    new_mx.m.push_back ({line_num++, new_ln});
            }

            new_mx.height = line_num;
            new_mx.width  = mx.width;
        }

        catch (...) { 
            Erase (new_mx); 
            throw; 
        }
        
        return new_mx;
    }

    void Erase (Sparse_mx& mx) noexcept {
        
        for (const auto& ln : mx.m) 
            if (ln.second) delete ln.second;

        mx.m = {};
        mx.height = 0;
        mx.width = 0;
    }
}

namespace sparse_line {

    Sparse_line *Input (std::size_t ln_width) {

        Sparse_line *ln = nullptr; 

        try {
            ln = new Sparse_line {};

            std::cout << "Enter items in line: ";

            for (std::size_t i = 0; i < ln_width; i++) {

                int val = Numbers::get_num<int>();
        
                if (val) {
                    List_pair pair = {i, val};
                    ln->push_back (pair);
                }
            }

            if (ln && ln->size() == 0) {
                delete (ln);
                ln = nullptr;
            }
        }

        catch (...) { 
            delete ln; 
            ln = nullptr;
            throw; 
        }

        return ln;
    }

    void Output (Sparse_line *ln, std::size_t width) noexcept {

        if (!ln) return;

        std::size_t cnt = 0;

        for (const auto& val : *ln) {
            for (; val.first > cnt; cnt++) 
                std::cout << 0 << " ";
            cnt = val.first + 1;
            
            std::cout << val.second << " "; 
        }

        for (; width > cnt; cnt++) 
            std::cout << 0 << " ";

        std::cout << std::endl;
    }

    void Output_empty (std::size_t width) noexcept {

        for (std::size_t i = 0; i < width; i++)
            std::cout << 0 << " ";
        std::cout << std::endl;
    }
}

