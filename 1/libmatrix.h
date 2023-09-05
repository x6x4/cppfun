#pragma once

#include <iostream>
#include <utility>
#include <vector>
#include <list>

//  Library for operations with sparse matrices

typedef std::pair<std::size_t, int> List_pair;
typedef std::list<List_pair> Sparse_line;
typedef std::vector<Sparse_line> Sparse_matrix;

namespace sparse_matrix {

    struct Sparse_mx {
        std::size_t width = 0;
        Sparse_matrix m = {};
    };

    Sparse_mx Input ();
    void Output (const char *msg, const Sparse_mx matrix);
    void Erase (Sparse_mx& matrix); 
}

namespace sparse_line {
    Sparse_line Input (size_t width);
}




