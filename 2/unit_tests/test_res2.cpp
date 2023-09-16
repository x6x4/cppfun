#define CATCH_CONFIG_MAIN

#include <sstream>
#include "../lib2/res_table.h"
#include <catch2/catch.hpp>


//  COMPLEX CLASS

TEST_CASE("Resource Table Constructors") {
    
    SECTION("DefaultConstructor") {
        Res_Table t;
        REQUIRE(t.vec.data == nullptr);
        REQUIRE(t.vec.cap  == 0);
        REQUIRE(t.vec.sz == 0);
    }

    SECTION("InitConstructor") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};

        REQUIRE(equal(t.vec.data[0], r1));
        REQUIRE(equal(t.vec.data[1], r2));
        REQUIRE(equal(t.vec.data[2], r3));
        REQUIRE(t.vec.cap == 3);
        REQUIRE(t.vec.sz == 3);
    }
    
    SECTION("CopyConstructor") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};
        Res_Table t1 = t;

        REQUIRE(equal(t1.vec.data[0], r1));
        REQUIRE(equal(t1.vec.data[1], r2));
        REQUIRE(equal(t1.vec.data[2], r3));
        REQUIRE(t1.vec.cap == 3);
        REQUIRE(t1.vec.sz == 3);

        REQUIRE(equal(t.vec.data[0], r1));
        REQUIRE(equal(t.vec.data[1], r2));
        REQUIRE(equal(t.vec.data[2], r3));
        REQUIRE(t.vec.cap == 3);
        REQUIRE(t.vec.sz == 3);
    }
    
    SECTION("MoveConstructor") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};
        Res_Table t1 = std::move(t);

        REQUIRE(equal(t1.vec.data[0], r1));
        REQUIRE(equal(t1.vec.data[1], r2));
        REQUIRE(equal(t1.vec.data[2], r3));
        REQUIRE(t1.vec.cap == 3);
        REQUIRE(t1.vec.sz == 3);
        
        REQUIRE(t.vec.data == nullptr);
        REQUIRE(t.vec.cap == 0);
        REQUIRE(t.vec.sz == 0);
    }
}

TEST_CASE("Resource table IO") {
    /*SECTION("INPUT") {
        Res input ("A", 5, 7, 10);
        std::stringstream in("B 6 8 9");
        in >> input;

        REQUIRE(input.get_name() == "B");
        REQUIRE(input.get_cons() == 6);
        REQUIRE(input.get_prod() == 8);
        REQUIRE(input.get_price() == 9);


        Res input2 ("A", 5, 7, 10);
        std::stringstream in2("B -6 8 9");
        
        REQUIRE_THROWS(in2 >> input);
    }*/
    SECTION("OUTPUT") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table output = {r1, r2, r3};

        std::stringstream out;
        out << output;

        REQUIRE(out.str() == "A 5 7 10\nB 2 6 4\nC 4 3 8\n");
    }
}

TEST_CASE("Resource Other") {

    SECTION("PROFIT") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};
        
        //REQUIRE(t.get_profit() == 140);

        
        //REQUIRE(r2.get_profit() == -112);
    }
    
    SECTION("MULTIPLY") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};

        t = t*7;
        REQUIRE(r1.get_cons() == 35);
        REQUIRE(r1.get_prod() == 49);
        REQUIRE(r2.get_cons() == 14);
        REQUIRE(r2.get_prod() == 42);
        REQUIRE(r3.get_cons() == 28);
        REQUIRE(r3.get_prod() == 21);
    }
}