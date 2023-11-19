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
    SECTION("Sequence container") {
        SECTION("From copies") {
            my_std::Vec<int> vec (5, 3);
            my_std::Vec<int> plain_vec {3, 3, 3, 3, 3};
            REQUIRE(vec == plain_vec);
            REQUIRE(std::distance(vec.begin(), vec.end()) == 5);
        }
        SECTION("From initializer list") {
            my_std::Vec<int> vec {1, 2, 3, 4};
            std::initializer_list<int> list {1, 2, 3, 4};
            REQUIRE(std::equal(vec.cbegin(), vec.cend(), list.begin()));
            REQUIRE(vec.size() == 4);

            std::initializer_list<int> list_to_assign {5, 6};
            vec = list_to_assign;
            REQUIRE(std::equal(vec.cbegin(), vec.cend(), list_to_assign.begin()));
            REQUIRE(vec.size() == 2);
        }
        SECTION("From range") {
            std::initializer_list<int> list {1, 2, 3, 4, 8};
            my_std::Vec<int> vec (list.begin(), list.end());
            REQUIRE(vec == my_std::Vec<int>(list));
            REQUIRE(std::distance(vec.begin(), vec.end()) == std::distance(list.begin(), list.end()));
        }
        SECTION("Clear") {
            my_std::Vec<int> vec {1, 2, 3, 4};
            vec.clear();
            REQUIRE(vec.empty());
            std::initializer_list<int> list {5, 6};
            vec = list;
            REQUIRE(vec == my_std::Vec<int>(list));
        }
        SECTION("Assignment") {
            my_std::Vec<int> vec {1, 2, 3, 4};
            std::initializer_list<int> list1 {5, 6};
            vec.assign(list1.begin(), list1.end());
            REQUIRE(vec == my_std::Vec<int> (list1));

            std::initializer_list<int> list2 {8, 9, 10, 11, 12, 13, 15, 87};
            vec.assign(list2);
            REQUIRE(vec == my_std::Vec<int> (list2));

            vec.assign(1, 7);
            REQUIRE(vec == my_std::Vec<int> (1, 7));
        }
        SECTION("Access") {
            my_std::Vec<int> vec ({1, 2, 3, 4});
            std::array arr {1, 2, 3, 4};
            for (size_t i = 0; i < vec.size(); i++) {
                REQUIRE(vec[i] == vec.at(i));
                REQUIRE(vec[i] == arr[i]);
            }
            REQUIRE_NOTHROW(vec[4]);
            REQUIRE_THROWS(vec.at(4));

            vec[2] = 5;
            vec.at(3) = 7;
            REQUIRE_THROWS(vec.at(11) = 9);
            REQUIRE(vec == my_std::Vec<int>{1, 2, 5, 7});
        }
        SECTION("Push back & capacity") {
            my_std::Vec<int> vec;
            REQUIRE(vec.capacity() == vec.size());
            vec.push_back(1);
            REQUIRE(vec == my_std::Vec<int>{1});
            REQUIRE(vec.capacity() == 1);

            vec.push_back(2);
            REQUIRE(vec == my_std::Vec<int>{1, 2});
            REQUIRE(vec.capacity() == 2);
            vec.push_back(3);
            REQUIRE(vec == my_std::Vec<int>{1, 2, 3});
            REQUIRE(vec.capacity() == 4);
        }
    }
}
