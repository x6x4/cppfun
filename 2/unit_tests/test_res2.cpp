#define CATCH_CONFIG_MAIN

#include <sstream>
#include "../lib2/res_table.h"
#include <catch2/catch.hpp>


//  COMPLEX CLASS


TEST_CASE("Resource Table Constructors") {
    
    SECTION("DefaultConstructor") {
        Res_Table t;
        REQUIRE_THROWS(t.vec[0]);
        REQUIRE(t.size() == 0);
    }

    SECTION("InitConstructor") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};

        REQUIRE(equal(t.vec[0], r1));
        REQUIRE(equal(t.vec[1], r2));
        REQUIRE(equal(t.vec[2], r3));
        REQUIRE(t.size() == 3);
    }
    
    SECTION("CopyConstructor") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};
        Res_Table t1 = t;

        REQUIRE(equal(t1.vec[0], r1));
        REQUIRE(equal(t1.vec[1], r2));
        REQUIRE(equal(t1.vec[2], r3));
        REQUIRE(t1.size() == 3);

        REQUIRE(equal(t.vec[0], r1));
        REQUIRE(equal(t.vec[1], r2));
        REQUIRE(equal(t.vec[2], r3));
        REQUIRE(t.size() == 3);
    }
    
    SECTION("MoveConstructor") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};
        Res_Table t1 = std::move(t);

        REQUIRE(equal(t1.vec[0], r1));
        REQUIRE(equal(t1.vec[1], r2));
        REQUIRE(equal(t1.vec[2], r3));
        REQUIRE(t1.size() == 3);
        
        REQUIRE_THROWS(t.vec[0]);
        REQUIRE(t.size() == 0);
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
        REQUIRE(t.get_profit() == 196);

        Res r4 ("A", 8, 1, 10);
        Res r5 ("B", 2, 5, 3);
        Res_Table t2 = {r4, r5};
        REQUIRE(t2.get_profit() == -427);
    }
    
    SECTION("MULTIPLY") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};

        t = t*7;
        REQUIRE(t.vec[0].get_cons() == 35);
        REQUIRE(t.vec[0].get_prod() == 49);
        REQUIRE(t.vec[1].get_cons() == 14);
        REQUIRE(t.vec[1].get_prod() == 42);
        REQUIRE(t.vec[2].get_cons() == 28);
        REQUIRE(t.vec[2].get_prod() == 21);
    }

    SECTION("GET_BY_NAME") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};

        REQUIRE(equal(t["A"], r1));
        REQUIRE_THROWS(t["a"]);
    }

    SECTION("RENAME") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r1, r2, r3};
        Res r4 ("D", 2, 6, 4);

        t.rename("B", "D");
        REQUIRE(equal(t["D"], r4));
        REQUIRE_THROWS(t.rename("B", "D"));
    }

    
}