#pragma once

#include <vector>
#include <list>


//----------------  Library for operations with sparse matrices  ----------------//


using List_pair = std::pair<std::size_t, int>;
using Sparse_line = std::list<List_pair>;

using Line_pair = std::pair<std::size_t, Sparse_line>;
using Sparse_matrix = std::list<Line_pair>;


namespace sparse_matrix {

    struct Sparse_mx {
        Sparse_matrix m = {};
        std::size_t height = 0;
        std::size_t width = 0;  
    };
    
    Sparse_mx Input ();
    void Output (const char *msg, const Sparse_mx &matrix) noexcept;
    Sparse_mx Create_modify (Sparse_mx &mx);
    void Erase (Sparse_mx &matrix) noexcept; 
}

namespace sparse_line {

    Sparse_line Input (std::size_t width);
    void Output (Sparse_line ln, std::size_t width) noexcept;
    void Output_empty (std::size_t width) noexcept;
}




