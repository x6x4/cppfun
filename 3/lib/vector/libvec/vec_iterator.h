/**
 * @file vec_iterator.h
 * @brief A custom contiguous iterator implementation.
 *
 * This file contains a custom contiguous iterator that is dedicated to use in custom vector template.
 * It meets the standard requirements of @a std::contiguous_iterator.
 * Warning: NO ALLOCATORS!
 */
 
 #pragma once
#include <bits/iterator_concepts.h>


namespace my_std {
    
    template<typename T, bool is_const>
    class vec_iterator {

        T* ptr;

        //  std::input_or_output_iterator
        T& operator++() { return ptr++; }

    };
};

static_assert(std::contiguous_iterator<my_std::vec_iterator<int, false>>);
static_assert(std::contiguous_iterator<my_std::vec_iterator<int, true>>);

