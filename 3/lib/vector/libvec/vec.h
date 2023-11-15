/**
 * @file vec.h
 * @brief A custom vector template container implementation.
 *
 * This file contains a sequence container that encapsulates dynamic C-style arrays (stripped std::vector).
 * It meets the standard requirements and provides access to elements through an custom vec_iterator iterator.
 * Warning: NO ALLOCATORS!
 */

#pragma once
#include <algorithm>
#include <bits/iterator_concepts.h>
#include <concepts>
#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <type_traits>
#include <iostream>

//#include "vec_iterator.h"


namespace my_std 
{
    template <typename T> 
    concept is_vec_element = std::is_copy_constructible_v<T> 
                          && std::is_copy_assignable_v<T>
                          && std::equality_comparable<T>
                          && std::destructible<T>;

    template <is_vec_element T> 
    struct Vec_base
    {
        typedef T* pointer;

        struct Vec_range
        {
            pointer start;
            pointer finish;
            pointer end_of_storage;

            constexpr 
            Vec_range() noexcept
            : start(), finish(), end_of_storage() { }

            constexpr 
            Vec_range(Vec_range &&_other) noexcept
            : start(_other.start), finish(_other.finish),
            end_of_storage(_other.end_of_storage)
            { _other.start = _other.finish = _other.end_of_storage = pointer(); }

            constexpr 
            Vec_range(const Vec_range &_other) noexcept = default;

            constexpr 
            Vec_range &operator=(const Vec_range &_other) noexcept = default;

            void swap (Vec_range &_other) {
                std::swap (start, _other.start);
                std::swap (finish, _other.finish);
                std::swap (end_of_storage, _other.end_of_storage);
            }
        };

        constexpr void create_storage(size_t n) {
            if (n > max_alloc_size)
                throw std::runtime_error ("cannot allocate so much storage");

            range.start = allocate(n);
            range.finish = range.start;
            range.end_of_storage = range.start + n;
        }

        constexpr Vec_base() noexcept (std::is_nothrow_default_constructible<T>()) { }
        constexpr Vec_base(Vec_base&& _rv) noexcept : range(std::move(_rv.range)) {};
    
        Vec_base(size_t n) : range() { 
            create_storage(n); 
            range.finish = range.end_of_storage;
        }

        constexpr ~Vec_base() noexcept {
            deallocate(range.start);
        }

        typedef __gnu_cxx::__numeric_traits<ptrdiff_t> ptr;

    public:
    
        Vec_range range;
        size_t max_alloc_size = ptr::__max / sizeof(T);

        constexpr pointer allocate(size_t n) { 
            return n != 0 ? new T[n] : pointer();
        }

        constexpr void deallocate(pointer p) {
            if (p) delete[] p;
        }

    };

    /**
    *  @brief A standard container which offers fixed time access to
    *  individual elements in any order.
    *
    *  @tparam T  Type of element.
    *
    *  Meets the requirements of a Container, SequenceContainer, 
    *  ContiguousContainer and ReversibleContainer (+).
    *
    *  It offers fast and efficient access to individual
    *  elements in any order and saves the user from worrying about
    *  memory and size allocation. Subscripting ([]) access is
    *  provided as with C-style arrays.
    */
    template<typename T>
        class Vec : protected Vec_base<T> {

            static_assert(std::is_same<typename std::remove_cv<T>::type, T>::value,
            "Vec must have a non-const, non-volatile value_type");

            typedef Vec_base<T>			     Base;
            typedef Base::pointer		  pointer;
            typedef const pointer	const_pointer;

            public:

        //  CONTAINER

            typedef T				   value_type;
            typedef T&		            reference;
            typedef const T&	  const_reference;
            typedef pointer iterator;
            typedef const_pointer const_iterator;
            typedef ptrdiff_t     difference_type;
            typedef size_t			    size_type;


            protected:
            using Base::allocate;
            using Base::deallocate;
            using Base::range;
            using Base::max_alloc_size;

            public:

            //  CONSTRUCTORS & DESTRUCTOR

            /**
            *  @brief  Creates a %Vec with no elements.
            *  @note   DefaultConstructible
            */
            Vec() = default;

            /**
            *  @brief  Creates a %Vec with default constructed elements.
            *  @param  n  The number of elements to initially create.
            *
            *  This constructor fills the %Vec with @a n default
            *  constructed elements.
            *
            *  @note @c Non-trivial constructor.
            */
            Vec(size_type n) : Base(n) { 
                for (auto &elm : *this) { elm = value_type(); }
            }   

            /**
            *  @brief  Builds a %Vec from an initializer list.
            *  @param  list  An initializer_list.
            *
            *  Create a %Vec consisting of copies of the elements in the
            *  initializer_list @a list.
            *
            *  @note @c Non-trivial constructor.
            */
            Vec (const std::initializer_list<T> &list) : Base (list.size()) {
                std::copy(list.begin(), list.end(), begin());
            }

            /**
            *  @brief  %Vec copy constructor.
            *  @param  copy  A %Vec of identical element type.
            *
            *  All the elements of @a copy are copied, but any unused capacity in
            *  @a copy  will not be copied
            *  (i.e. capacity() == size() in the new %Vec).
            *  @note CopyConstructible
            *  @note @c T must be CopyInsertable 
            */
            constexpr 
            Vec(const Vec &to_copy) : Base (to_copy.size()) { 
                std::copy(to_copy.begin(), to_copy.end(), begin()); 
            }

            /**
            *  @brief  %Vec move constructor.
            *
            *  The newly-created %Vec contains the exact contents of the
            *  moved instance.
            * @note  Swappable = @c std::is_nothrow_move_constructible_v && std::is_nothrow_move_assignable_v
            */
            Vec(Vec&&) noexcept = default;

            /**
            *  The dtor only erases the elements, and note that if the
            *  elements themselves are pointers, the pointed-to memory is
            *  not touched in any way.  Managing the pointer is the user's
            *  responsibility.
            */
            constexpr 
            ~Vec() noexcept {
                std::destroy(range.start, range.finish);
            }

            /**
            *  @brief  %Vec copy assignment operator.
            *  @param  copy  A %Vec of identical element type.
            *
            *  All the elements of @a copy are copied, but any unused capacity in
            *  @a copy will not be copied.
            */
            constexpr Vec& operator=(const Vec& to_copy) {
                Vec tmp (to_copy);
                range.swap(tmp.range);
                return *this;
            }
            
            /**
            *  @brief  %Vec move assignment operator.
            *  @param  move  A %Vec of identical element type.
            *
            *  The contents of @a move are moved into this %Vec.
            *  @note Swappable = std::is_nothrow_move_constructible_v && @c std::is_nothrow_move_assignable_v
            */
            constexpr Vec& operator=(Vec&& to_move) noexcept {
                swap(to_move);
                return *this;
            }

            /**
            *  @brief  Swaps data with another %Vec.
            *  @param  _other  A %Vec of the same element type.
            *
            *  This exchanges the elements between two vectors in constant time.
            *  (Three pointers, so it should be quite fast.)
            */
            void swap(Vec &_other) {
                range.swap(_other.range);
            }

            //  ITERATORS

            /**
            *  Returns a read/write iterator that points to the first
            *  element in the %Vec.  Iteration is done in ordinary
            *  element order.
            */
            [[nodiscard]] constexpr iterator 
            begin() noexcept
            { return iterator(range.start); }

            /**
            *  Returns a read-only (constant) iterator that points to the
            *  first element in the %Vec.  Iteration is done in ordinary
            *  element order.
            */
            [[nodiscard]] constexpr const_iterator
            begin() const noexcept
            { return const_iterator(range.start); }; 

            /**
            *  Returns a read/write iterator that points one past the last
            *  element in the %Vec.  Iteration is done in ordinary
            *  element order.
            */
            [[nodiscard]] constexpr iterator 
            end() noexcept
            { return iterator(range.finish); }

            /**
            *  Returns a read-only (constant) iterator that points one past
            *  the last element in the %Vec.  Iteration is done in
            *  ordinary element order.
            */
            [[nodiscard]] constexpr const_iterator
            end() const noexcept
            { return const_iterator(range.finish); }

            /**
            *  Returns a read-only (constant) iterator that points to the
            *  first element in the %Vec.  Iteration is done in ordinary
            *  element order.
            */
            [[nodiscard]] constexpr const_iterator 
            cbegin() const noexcept
            { return const_iterator(range.start); }

            /**
            *  Returns a read-only (constant) iterator that points one past
            *  the last element in the %Vec.  Iteration is done in
            *  ordinary element order.
            */
            [[nodiscard]] constexpr const_iterator 
            cend() const noexcept
            { return const_iterator(range.finish); }


            //  GENERAL METHODS

            /**  Returns the number of elements in the %Vec.  */
            [[nodiscard]] constexpr size_type 
            size() const noexcept
            { return size_type(range.finish - range.start); }

            /**
            *  Returns true if the %Vec is empty.  
            *  (Thus begin() would equal end().)
            */
            [[nodiscard]] constexpr bool
            empty() const noexcept
            { return begin() == end(); }

            /**  Returns the size() of the largest possible %Vec.  */
            [[nodiscard]] constexpr size_type 
            max_size() const noexcept { 
                return max_alloc_size; 
            }

        };

        /**
        *  @brief  Vector equality comparison.
        *  @param  vec1  A %Vec.
        *  @param  vec2  A %Vec of the same type as @a vec1
        *  @return  True iff the size and elements of the vectors are equal.
        *
        *  This is an equivalence relation.  It is linear in the size of the
        *  vectors.  Vectors are considered equivalent if their sizes are equal,
        *  and if corresponding elements compare equal.
        */
        template<typename T>
        constexpr inline bool
        operator==(const Vec<T> &vec1, const Vec<T> &vec2) { 
            return (vec1.size() == vec2.size()
            && std::equal(vec1.begin(), vec1.end(), vec2.begin()));
        };

        /// See my_std::Vec::swap()
        template<typename T>
        constexpr inline void
        swap (Vec<T> &vec1, Vec<T> &vec2) {
            vec1.swap(vec2);
        }

}; // namespace my_std