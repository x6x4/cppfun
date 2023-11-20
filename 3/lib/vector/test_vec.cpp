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

            auto default_vec2 = my_std::Vec<std::string>();
            REQUIRE(default_vec2.size() == 0);
            REQUIRE(default_vec2.empty());
            REQUIRE(default_vec2.begin() == default_vec2.end());            
        }
        SECTION("Sized") {
            my_std::Vec<int> sized_vec(4);
            REQUIRE(sized_vec.size() == 4);
            REQUIRE(!sized_vec.empty());
            for (auto elm : sized_vec) 
                REQUIRE(elm == 0);

            my_std::Vec<std::string> sized_vec2(4);
            REQUIRE(sized_vec2.size() == 4);
            REQUIRE(!sized_vec2.empty());
            for (auto elm : sized_vec2) 
                REQUIRE(elm == "");
        }
        SECTION("Initializer list") {
            my_std::Vec<int> vec {1, 2, 3, 4};
            std::initializer_list<int> list {1, 2, 3, 4};
            REQUIRE(std::equal(vec.cbegin(), vec.cend(), list.begin()));
            REQUIRE(vec.size() == 4);

            my_std::Vec<std::string> _vec_init{ "1", "2", "3", "4" };
            std::initializer_list<std::string> _list { "1", "2", "3", "4" };
            REQUIRE(std::equal(_vec_init.cbegin(), _vec_init.cend(), _list.begin()));
            REQUIRE(_vec_init.size() == 4);
        }
        SECTION("Copy") {
            my_std::Vec<int> vec {1, 2, 3, 4};
            REQUIRE(vec == my_std::Vec(vec));

            my_std::Vec<std::string> vec2 {"sdddd", "fjjfw", "", "sl89"};
            REQUIRE(vec2 == my_std::Vec(vec2));
        }
        SECTION("Move") {
            my_std::Vec<int> vec1 {1, 2, 3, 4};
            my_std::Vec<int> vec2 (vec1);
            my_std::Vec<int> vec (std::move (vec1));
            REQUIRE(vec == vec2);

            my_std::Vec<std::string> _vec1 {"fsfdfw", "dddd", "93", "ddd"};
            my_std::Vec<std::string> _vec2 (_vec1);
            my_std::Vec<std::string> _vec (std::move (_vec1));
            REQUIRE(_vec == _vec2);
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

            my_std::Vec<std::string> _vec(10);
            my_std::Vec<std::string> _to_copy {"2", "", "eee", "qk9292"};
            my_std::Vec<std::string> _backup (_to_copy);
            _vec = _to_copy;
            REQUIRE(_vec == _to_copy);
            REQUIRE(_to_copy == _backup);
        }
        SECTION("Move") {
            my_std::Vec<int> to_move {1, 2, 3, 4};
            my_std::Vec<int> backup (to_move);
            my_std::Vec<int> vec(2);
            vec = std::move (to_move);
            REQUIRE(vec == backup);

            my_std::Vec<std::string> _to_move {"weq", "fjjjj", "2", ""};
            my_std::Vec<std::string> _backup (_to_move);
            my_std::Vec<std::string> _vec(2);
            _vec = std::move (_to_move);
            REQUIRE(_vec == _backup);
        }
    }
    SECTION("Other") {
        SECTION("Max size") {
            REQUIRE_THROWS(my_std::Vec<int>(std::numeric_limits<size_t>::max()));
            my_std::Vec<int> vec(10000);
            REQUIRE(vec.max_size() >= vec.size());

            REQUIRE_THROWS(my_std::Vec<std::string>(std::numeric_limits<size_t>::max()));
            my_std::Vec<std::string> _vec(10000);
            REQUIRE(_vec.max_size() >= _vec.size());
        }
        SECTION("!=") {
            my_std::Vec<int> vec1 {1, 2, 3, 4};
            my_std::Vec<int> vec2 {8, 9, 4};
            my_std::Vec<int> vec2_ {8, 9, 5};
            my_std::Vec<int> vec3 {1, 2, 3, 4};
            REQUIRE(vec1 != vec2);
            REQUIRE(vec2_ != vec2);
            REQUIRE(vec3 == vec1);

            my_std::Vec<std::string> _vec1 {"qqq", "qww", "w", "wwwww"};
            my_std::Vec<std::string> _vec2 {"22", "er", "111"};
            my_std::Vec<std::string> _vec2_ {"22", "er", "11"};
            my_std::Vec<std::string> _vec3 {"22", "er", "111"};
            REQUIRE(_vec1 != _vec2);  
            REQUIRE(_vec2_ != _vec2); 
            REQUIRE(_vec3 == _vec2);   
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

            my_std::Vec<std::string> _vec1 {"1aaa", "ss2", "ew3"};
            auto _backup1 = _vec1;
            my_std::Vec<std::string> _vec2 {"8a", "5e", "rr9", "4ff", "fg6"};
            auto _backup2 = _vec2;
            _vec1.swap(_vec2);
            REQUIRE(_vec1 == _backup2);
            REQUIRE(_vec2 == _backup1);
            my_std::swap(_vec1, _vec2);
            REQUIRE(_vec1 == _backup1);
            REQUIRE(_vec2 == _backup2);
        }
    }
    SECTION("Sequence container") {
        SECTION("From copies") {
            my_std::Vec<int> vec (5, 3);
            my_std::Vec<int> plain_vec {3, 3, 3, 3, 3};
            REQUIRE(vec == plain_vec);
            REQUIRE(std::distance(vec.begin(), vec.end()) == 5);

            my_std::Vec<std::string> _vec(5, "3d");
            my_std::Vec<std::string> _plain_vec{"3d", "3d", "3d", "3d", "3d"};
            REQUIRE(_vec == _plain_vec);
            REQUIRE(std::distance(_vec.begin(), _vec.end()) == 5);
        }
        SECTION("From initializer list") {
            SECTION("Assign") {
                my_std::Vec<int> vec {1, 2, 3, 4};
                std::initializer_list<int> list_to_assign {5, 6};
                vec = list_to_assign;
                REQUIRE(std::equal(vec.cbegin(), vec.cend(), list_to_assign.begin()));
                REQUIRE(vec.size() == 2);

                my_std::Vec<std::string> _vec_init{ "1", "2", "3", "4" };
                std::initializer_list<std::string> _list_to_assign { "5", "6" };
                _vec_init = _list_to_assign;
                REQUIRE(std::equal(_vec_init.cbegin(), _vec_init.cend(), _list_to_assign.begin()));
                REQUIRE(_vec_init.size() == 2);
            }
        }
        SECTION("From range") {
            std::initializer_list<int> list {1, 2, 3, 4, 8};
            my_std::Vec<int> vec (list.begin(), list.end());
            REQUIRE(vec == my_std::Vec<int>(list));
            REQUIRE(std::distance(vec.begin(), vec.end()) 
            == std::distance(list.begin(), list.end()));

            std::initializer_list<std::string> _list_new { "1", "2", "3", "4", "8" };
            my_std::Vec<std::string> _vec_construct(_list_new.begin(), _list_new.end());
            REQUIRE(_vec_construct == my_std::Vec<std::string>(_list_new));
            REQUIRE(std::distance(_vec_construct.begin(), _vec_construct.end()) 
            == std::distance(_list_new.begin(), _list_new.end()));
        }
        SECTION("Clear") {
            my_std::Vec<int> vec {1, 2, 3, 4};
            vec.clear();
            REQUIRE(vec.empty());
            std::initializer_list<int> list {5, 6};
            vec = list;
            REQUIRE(vec == my_std::Vec<int>(list));

            my_std::Vec<std::string> _vec{ "1", "2", "3", "4" };
            _vec.clear();
            REQUIRE(_vec.empty());
            std::initializer_list<std::string> _list{ "5", "6" };
            _vec = _list;
            REQUIRE(_vec == my_std::Vec<std::string>(_list));
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


            my_std::Vec<std::string> _vec_init{ "1", "2", "3", "4" };
            std::initializer_list<std::string> _list1{ "5", "6" };
            _vec_init.assign(_list1.begin(), _list1.end());
            REQUIRE(_vec_init == my_std::Vec<std::string>(_list1));

            std::initializer_list<std::string> _list2{ "8", "9", "10", "11", "12", "13", "15", "87" };
            _vec_init.assign(_list2);
            REQUIRE(_vec_init == my_std::Vec<std::string>(_list2));

            _vec_init.assign(1, "7");
            REQUIRE(_vec_init == my_std::Vec<std::string>(1, "7"));
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


            my_std::Vec<std::string> _vec_construct({ "1", "2", "3", "4" });
            std::array<std::string, 4> _arr{ "1", "2", "3", "4" };
            for (size_t i = 0; i < _vec_construct.size(); i++) {
                REQUIRE(_vec_construct[i] == _vec_construct.at(i));
                REQUIRE(_vec_construct[i] == _arr[i]);
            }
            REQUIRE_NOTHROW(_vec_construct[4]);
            REQUIRE_THROWS(_vec_construct.at(4));

            _vec_construct[2] = "5";
            _vec_construct.at(3) = "7";
            REQUIRE_THROWS(_vec_construct.at(11) = "9");
            REQUIRE(_vec_construct == my_std::Vec<std::string>{"1", "2", "5", "7"});
        }
        SECTION("Push back & capacity") {
            my_std::Vec<int> vec;
            REQUIRE(vec.capacity() == vec.size());
            int a = 1;
            vec.push_back(a);
            REQUIRE(vec == my_std::Vec<int>{1});
            REQUIRE(vec.capacity() == 1);

            vec.push_back(2);
            REQUIRE(vec == my_std::Vec<int>{1, 2});
            REQUIRE(vec.capacity() == 2);
            int c = 3;
            vec.push_back(c);
            REQUIRE(vec == my_std::Vec<int>{1, 2, 3});
            REQUIRE(vec.capacity() == 4);


            my_std::Vec<std::string> _vec;
            REQUIRE(_vec.capacity() == _vec.size());
            _vec.push_back("1");
            REQUIRE(_vec == my_std::Vec<std::string>{"1"});
            REQUIRE(_vec.capacity() == 1);

            std::string b = "2";
            _vec.push_back(b);
            REQUIRE(_vec == my_std::Vec<std::string>{"1", "2"});
            REQUIRE(_vec.capacity() == 2);
            _vec.push_back("3");
            REQUIRE(_vec == my_std::Vec<std::string>{"1", "2", "3"});
            REQUIRE(_vec.capacity() == 4);
        }
    }
}
