#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <limits>
#define CATCH_CONFIG_MAIN 

#include "libvec/vec.h"
#include <catch2/catch.hpp>
#include <array>

//  destructor is tested by valgrind
//  empty is tested by default & sized constructor
//  begin, end and == are used everywhere

TEST_CASE("Vec") {
  SECTION("Constructors") {
    SECTION("Default") {
        auto default_vec = my_std::Vec<int>();
        REQUIRE(default_vec.size() == 0);
        REQUIRE(default_vec.empty());
        REQUIRE(default_vec.begin() == default_vec.end());
    }
    SECTION("Sized") {
        my_std::Vec<int> sized_vec(4);
        REQUIRE(sized_vec.size() == 4);
        REQUIRE(!sized_vec.empty());
        for (auto elm : sized_vec) 
            REQUIRE(elm == 0);
    }
    SECTION("Initializer list") {
        my_std::Vec<int> vec {1, 2, 3, 4};
        std::initializer_list<int> list {1, 2, 3, 4};
        REQUIRE(std::equal(vec.cbegin(), vec.cend(), list.begin()));
    }
    SECTION("Copy") {
        my_std::Vec<int> vec {1, 2, 3, 4};
        REQUIRE(vec == my_std::Vec(vec));
    }
    SECTION("Move") {
        my_std::Vec<int> vec1 {1, 2, 3, 4};
        my_std::Vec<int> vec2 (vec1);
        my_std::Vec<int> vec (std::move (vec1));
        REQUIRE(vec == vec2);
    }
  }
  SECTION("Assignment") {
    SECTION("Copy") {
        my_std::Vec<int> vec(10);
        my_std::Vec<int> to_copy {1, 2, 3, 4};
        my_std::Vec<int> backup (to_copy);
        vec = to_copy;
        REQUIRE(vec == to_copy);
        REQUIRE(to_copy == backup);
    }
    SECTION("Move") {
        my_std::Vec<int> to_move {1, 2, 3, 4};
        my_std::Vec<int> backup (to_move);
        my_std::Vec<int> vec(2);
        vec = std::move (to_move);
        REQUIRE(vec == backup);
    }
  }
  SECTION("Other") {
    SECTION("Max size") {
        REQUIRE_THROWS(my_std::Vec<int>(std::numeric_limits<size_t>::max()));
        my_std::Vec<int> vec(10000);
        REQUIRE(vec.max_size() >= vec.size());
    }
    SECTION("!=") {
        my_std::Vec<int> vec1 {1, 2, 3, 4};
        my_std::Vec<int> vec2 {8, 2, 9, 4};
        REQUIRE(vec1 != vec2);
    }
    SECTION("Swap") {
        my_std::Vec<int> vec1 {1, 2, 3};
        auto backup1 = vec1;
        my_std::Vec<int> vec2 {8, 5, 9, 4, 6};
        auto backup2 = vec2;
        vec1.swap(vec2);
        REQUIRE(vec1 == backup2);
        REQUIRE(vec2 == backup1);
        my_std::swap(vec1, vec2);
        REQUIRE(vec1 == backup1);
        REQUIRE(vec2 == backup2);
    }
  }
}
