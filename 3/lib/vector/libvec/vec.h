/**
 * @file vec.h
 * @brief A custom vector template container implementation.
 *
 * This file contains a sequence container that encapsulates dynamic C-style arrays (stripped std::vector).
 * It meets the standard requirements and provides access to elements through an custom vec_iterator iterator.
 * Warning: NO ALLOCATORS!
 */

#pragma once
#include "fwd_vec.h"
#include <cstdlib>
#include <iterator>
#include <memory>
#include <new>
#include <stdexcept>
#include <type_traits>


namespace my_std 
{
    template <typename T> 
    concept is_vec_element = std::destructible<T>;

    template <is_vec_element T> 
    struct Vec_base
    {
        typedef T* pointer;
        typedef __gnu_cxx::__numeric_traits<ptrdiff_t> ptr_limits;
        size_t max_alloc_size = ptr_limits::__max / sizeof(T);

        struct Vec_range
        {
            pointer start = pointer();
            pointer finish = pointer();
            pointer end_of_storage = pointer();

            constexpr 
            Vec_range() noexcept = default;

            constexpr 
            Vec_range(Vec_range &&_other) noexcept
            : start(_other.start), finish(_other.finish),
            end_of_storage(_other.end_of_storage)
            { _other.start = _other.finish = _other.end_of_storage = pointer(); }

            constexpr 
            Vec_range &operator=(const Vec_range &_other) noexcept = default;

            void swap (Vec_range &_other) 
            { std::swap(*this, _other); }
        } range;

        constexpr Vec_base() noexcept (std::is_nothrow_default_constructible<T>()) { }
        constexpr Vec_base(Vec_base&& _rv) noexcept = default;

        Vec_base(size_t _n) : range() { 
            create_storage(_n);
            init_storage(_n); 
        } 

        Vec_base(size_t _n, const T& _val) : range() { 
            create_storage(_n);
            range.finish = std::uninitialized_fill_n(range.start, _n, _val);
        } 

        constexpr ~Vec_base() noexcept 
        { deallocate(range.start); }

    private:

        constexpr void create_storage(size_t n) {
            if (n > max_alloc_size)
                throw std::runtime_error ("cannot allocate so much storage");

            range.start = allocate(n);
            range.finish = range.start;
            range.end_of_storage = range.start + n;
        }

        void init_storage (size_t _n) 
        requires std::is_default_constructible_v<T>{
            range.finish = 
            std::uninitialized_value_construct_n(range.start, _n);
        }

        void init_storage (size_t _n) { range.finish = range.end_of_storage; }


        constexpr pointer allocate(size_t n)
        requires std::is_default_constructible_v<T> {
            return n != 0 ? 
            static_cast<pointer>(new T[n]) : pointer();
        }

        constexpr pointer allocate(size_t n) {
            return n != 0 ? 
            reinterpret_cast<pointer>(new char[n*sizeof(T)]) : pointer();
        }

        constexpr void deallocate(pointer p) 
        { if (p) delete[] p; }
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
        class Vec : private Vec_base<T> {

            static_assert(std::is_same<typename std::remove_cv<T>::type, T>::value,
            "Vec must have a non-const, non-volatile value_type");

            typedef Vec_base<T> Base; 
            typedef Base::pointer		  pointer;
            typedef const pointer	const_pointer;

            public:

        //  CONTAINER

            typedef T				           value_type;
            typedef value_type&		            reference;
            typedef const value_type&	  const_reference;
            typedef pointer                      iterator;
            typedef const_pointer          const_iterator;
            typedef ptrdiff_t             difference_type;
            typedef size_t			            size_type;

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
            Vec(size_type _n) : Base(_n) {}  

            /**
            *  @brief  Creates a %Vec with copies of an exemplar element.
            *  @param  _n  The number of elements to initially create.
            *  @param  _val  An element to copy.
            *
            *  This constructor fills the %Vec with @a _n copies of @a _val.
            */
            constexpr
            Vec (size_type _n, const value_type& _val) : Base(_n, _val) {} 

            /**
            *  @brief  Builds a %Vec from an initializer list.
            *  @param  _l  An initializer_list.
            *
            *  Create a %Vec consisting of copies of the elements in the
            *  initializer_list @a _l.
            *
            *  @note @c Non-trivial constructor.
            */
            Vec (const std::initializer_list<value_type> &_l) : Base (_l.size()) 
            { std::copy(_l.begin(), _l.end(), begin()); }

            /**
            *  @brief  Builds a %Vec from a range.
            *  @param  _first  An input iterator.
            *  @param  _last  An input iterator.
            *
            *  Create a %Vec consisting of copies of the elements from
            *  [first,last).
            */
            template<std::input_iterator _It>
            constexpr   
            Vec(_It _first, _It _last)
            requires std::constructible_from<T, std::iter_reference_t<_It>> :
            Base (std::distance(_first, _last))
            { std::copy(_first, _last, begin()); }

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
                *this = Vec(to_copy.begin(), to_copy.end());
            }

            /**
            *  @brief  %Vec move constructor.
            *
            *  The newly-created %Vec contains the exact contents of the
            *  moved instance.
            * @note  Swappable = @c std::is_nothrow_move_constructible_v && std::is_nothrow_move_assignable_v
            */
            constexpr 
            Vec(Vec &&to_move) noexcept = default;

            /**
            *  @brief  %Vec copy assignment operator.
            *  @param  copy  A %Vec of identical element type.
            *
            *  All the elements of @a copy are copied, but any unused capacity in
            *  @a copy will not be copied.
            */
            constexpr 
            Vec& operator=(const Vec& to_copy) {
                Vec tmp (to_copy);
                swap(tmp);
                return *this;
            }
            
            /**
            *  @brief  %Vec move assignment operator.
            *  @param  move  A %Vec of identical element type.
            *
            *  The contents of @a move are moved into this %Vec.
            *  @note Swappable = std::is_nothrow_move_constructible_v && @c std::is_nothrow_move_assignable_v
            */
            constexpr 
            Vec& operator=(Vec &&to_move) noexcept {
                swap(to_move);
                return *this;
            }

            /**
            *  The dtor only erases the elements, and note that if the
            *  elements themselves are pointers, the pointed-to memory is
            *  not touched in any way.  Managing the pointer is the user's
            *  responsibility.
            */
            constexpr 
            ~Vec() noexcept = default;

            //  ITERATORS

            /**
            *  Returns a read/write iterator that points to the first
            *  element in the %Vec.  Iteration is done in ordinary
            *  element order.
            */
            [[nodiscard]] constexpr iterator 
            begin() noexcept
            { return iterator(this->range.start); }

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

            //  CAPACITY

            /**  Returns the number of elements in the %Vec.  */
            [[nodiscard]] constexpr inline size_type 
            size() const noexcept
            { return size_type(range.finish - range.start); }

            /**
            *  Returns the total number of elements that the %Vec can
            *  hold before needing to allocate more memory.
            */
            [[nodiscard]] constexpr inline size_type 
            capacity() const noexcept
            { return size_type(range.end_of_storage - range.start); }

            /**
            *  Returns true if the %Vec is empty.  
            *  (Thus begin() would equal end().)
            */
            [[nodiscard]] constexpr inline bool
            empty() const noexcept
            { return begin() == end(); }

            [[nodiscard]] constexpr inline bool 
            full() const noexcept 
            { return size() == capacity(); }

            /**  Returns the size() of the largest possible %Vec.  */
            [[nodiscard]] constexpr size_type 
            max_size() const noexcept { 
                return max_alloc_size; 
            }

            //  MODIFIERS

            /**
            *  @brief  Swaps data with another %Vec.
            *  @param  _other  A %Vec of the same element type.
            *
            *  This exchanges the elements between two vectors in constant time.
            *  (Three pointers, so it should be quite fast.)
            */
            void swap(Vec &_other) {
                range.swap(_other.range);
                std::swap(max_alloc_size, _other.max_alloc_size);
            }


        //  SEQUENCE CONTAINER

            /*  absent because of (from stl_vector.h):
            *  Note that this kind of operation could be expensive for a %Vec
            *  and if it is frequently used the user should consider using
            *  std::list.

            //  a.emplace(p, args) 
            *  This function will insert an object of type T constructed
            *  with T(std::forward<Args>(args)...) before the specified location.

            //  a.insert(p, t)
            *  This function will insert a copy of the given value before
            *  the specified location. 

            //  a.insert(p, rv)
            *  This function will insert a copy of the given rvalue before
            *  the specified location. 
            
            //  a.insert(p, n, t)
            *  This function will insert a specified number of copies of
            *  the given data before the location specified by @a position.

            //  a.insert(p, i, j)
            *  This function will insert copies of the data in the range
            *  [__first,__last) into the %Vec before the location specified
            *  by @a pos.

            //  a.insert(p, il)
            *  This function will insert copies of the data in the
            *  initializer_list @a l into the %Vec before the location
            *  specified by @a position.

            //  a.erase(q)
            *  This function will erase the element at the given position and thus
            *  shorten the %Vec by one.

            //  a.erase(q1, q2)
            *  This function will erase the elements in the range
            *  [__first,__last) and shorten the %Vec accordingly.
            */

            //  MODIFIERS

            /**
            *  Erases all the elements.  Note that this function only erases the
            *  elements, and that if the elements themselves are pointers, the
            *  pointed-to memory is not touched in any way.  Managing the pointer is
            *  the user's responsibility.
            */
            constexpr void 
            clear () noexcept 
            { _assign(Vec()); }

            /**
            *  @brief  Assigns a range to a %Vec.
            *  @param  _first  An input iterator.
            *  @param  _last   An input iterator.
            *
            *  This function fills a %Vec with copies of the elements in the
            *  range [_first,_last).
            *
            *  Note that the assignment completely changes the %Vec and
            *  that the resulting %Vec's size is the same as the number
            *  of elements assigned.
            */
            template<std::input_iterator _It>
            constexpr void
            assign(_It _first, _It _last)
            { _assign(Vec(_first, _last)); }

            /**
            *  @brief  Assigns an initializer list to a %Vec.
            *  @param  _l  An initializer_list.
            *
            *  This function fills a %Vec with copies of the elements in the
            *  initializer list @a _l.
            *
            *  Note that the assignment completely changes the %Vec and
            *  that the resulting %Vec's size is the same as the number
            *  of elements assigned.
            */
            constexpr void
            assign(const std::initializer_list<value_type> &_l)
            { _assign(_l); }

            /**
            *  @brief  Assigns a given value to a %Vec.
            *  @param  _n  Number of elements to be assigned.
            *  @param  _val  Value to be assigned.
            *
            *  This function fills a %Vec with @a _n copies of the given
            *  value.  Note that the assignment completely changes the
            *  %Vec and that the resulting %Vec's size is the same as
            *  the number of elements assigned.
            */
            constexpr void
            assign(size_type _n, const value_type& _val)
            { _assign(Vec(_n, _val)); }

        private:

            //  Called by functions assigning to existing vector
            void 
            _assign (Vec to_assign) 
            { swap (to_assign); }


        public:

            //  OPTIONAL

            //  element access

            /**
            *  @brief  Subscript access to the data contained in the %Vec.
            *  @param _n The index of the element for which data should be
            *  accessed.
            *  @return  Read/write reference to data.
            *
            *  This operator allows for easy, array-style, data access.
            *  Note that data access with this operator is unchecked and
            *  out_of_range lookups are not defined. (For checked lookups
            *  see at().)
            */
            [[nodiscard]] constexpr reference
            operator[](size_type _n) noexcept
            { return *(range.start + _n); }

            /**
            *  @brief  Subscript access to the data contained in the %Vec.
            *  @param _n The index of the element for which data should be
            *  accessed.
            *  @return  Read-only (constant) reference to data.
            *
            *  This operator allows for easy, array-style, data access.
            *  Note that data access with this operator is unchecked and
            *  out_of_range lookups are not defined. (For checked lookups
            *  see at().)
            */
            [[nodiscard]] constexpr const_reference
            operator[](size_type _n) const noexcept
            { return *(range.start + _n); }

            /**
            *  @brief  Provides access to the data contained in the %Vec.
            *  @param _n The index of the element for which data should be
            *  accessed.
            *  @return  Read/write reference to data.
            *  @throw  std::out_of_range  If @a _n is an invalid index.
            *
            *  This function provides for safer data access.  The parameter
            *  is first checked that it is in the range of the vector.  The
            *  function throws out_of_range if the check fails.
            */
            [[nodiscard]] constexpr reference
            at(size_type _n) {
                range_check(_n);
                return (*this)[_n];
            }

            /**
            *  @brief  Provides access to the data contained in the %Vec.
            *  @param _n The index of the element for which data should be
            *  accessed.
            *  @return  Read-only (constant) reference to data.
            *  @throw  std::out_of_range  If @a _n is an invalid index.
            *
            *  This function provides for safer data access.  The parameter
            *  is first checked that it is in the range of the vector.  The
            *  function throws out_of_range if the check fails.
            */
            [[nodiscard]] constexpr const_reference
            at(size_type _n) const {
                range_check(_n);
                return (*this)[_n];
            }

        private:

            /// Safety check used only from at().
            constexpr void
            range_check(size_type _n) const {
                if (_n >= size()) {
                    std::string str = 
                    fmt::format("Vec::range_check: _n (which is {}) " 
                    ">= size() (which is {})", _n, size());

                    throw std::logic_error(str);
                }  
            };

        public:

            //  modifiers

            /**
            *  @brief  Add data to the end of the %Vec.
            *  @param  _val  Data to be added.
            *
            *  This is a typical stack operation.  The function creates an
            *  element at the end of the %Vec and assigns the given data
            *  to it.  Due to the nature of a %Vec this operation can be
            *  done in constant time if the %Vec has preallocated space
            *  available.
            */
            constexpr void
            push_back(const value_type &_val) {
                if (full()) _resize();
                *(range.finish++) = _val;
            }

            constexpr void
            push_back(value_type&& _val) {
                if (full()) _resize();
                *(range.finish++) = std::move(_val);
            }

            private:

            void _resize () {

                size_type old_sz = size();
                size_type new_sz = old_sz ? 2*old_sz : 1;

                Vec buf (new_sz);
                std::move(begin(), end(), buf.begin());

                buf.range.finish = buf.range.start + old_sz;
                swap(buf);
            }

        };  //  Vec

        //  NON-MEMBER FUNCTIONS

        template<typename T>
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
        constexpr inline bool
        operator==(const Vec<T> &vec1, const Vec<T> &vec2) 
        requires std::equality_comparable<T> { 
            return (vec1.size() == vec2.size()
            && std::equal(vec1.begin(), vec1.end(), vec2.begin()));
        };

        /// See my_std::Vec::swap()
        template<typename T>
        constexpr inline void
        swap (Vec<T> &vec1, Vec<T> &vec2) {
            vec1.swap(vec2);
        }

};  // namespace my_std