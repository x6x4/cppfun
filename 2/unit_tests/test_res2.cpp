#define CATCH_CONFIG_MAIN

#include <sstream>
#include "../lib2/res_table.h"
#include <catch2/catch.hpp>


//  COMPLEX CLASS


TEST_CASE("Resource Table Constructors") {
    
    SECTION("DefaultConstructor") {
        Res_Table t;
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
    SECTION("INPUT") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        
        Res_Table input;
        std::stringstream in("C 4 3 8\nA 5 7 10\nB 2 6 4");
        in >> input;

        REQUIRE(input.size()==3);
        REQUIRE(equal(input["A"], r1));
        REQUIRE(equal(input["B"], r2));
        REQUIRE(equal(input["C"], r3));

        Res_Table input2;
        std::stringstream in2("brrrrrrrrr");
        std::cout << input2;
        REQUIRE_THROWS(in2 >> input2);  
    }

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

TEST_CASE("Resource table Getters") {

    SECTION("CHECK") {
        Res_Table empty;
        REQUIRE(empty.state() == EMPTY);

        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("D", 4, 3, 8);
        Res_Table t = {r1, r2, r3};
        REQUIRE(t.state() == FULL);

        t.del("A");
        REQUIRE(t.state() == PART);

        t.del("B");
        REQUIRE(t.state() == PART);
        t.del("D");
        REQUIRE(t.state() == EMPTY);
    }

    SECTION("GET_BY_NAME") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("D", 4, 3, 8);
        Res_Table t = {r1, r2, r3};

        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r2));
        REQUIRE(equal(t["D"], r3));
        REQUIRE_THROWS(t["a"]);
        REQUIRE_THROWS(t["C"]);
    }

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
}

TEST_CASE("Resource table Setters") {

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

}

TEST_CASE("Resource table Other") {
    
    SECTION("ADD") {
        Res_Table empty;
        Res r ("R1", 8, 5, 4);
        Res rr ("R2", 80, 50, 40);
        empty+=r;
        REQUIRE(empty.size() == 1);
        REQUIRE(equal(empty["R1"], r));
        empty+=rr;
        REQUIRE(empty.size() == 2);
        REQUIRE(equal(empty["R1"], r));
        REQUIRE(equal(empty["R2"], rr));

        Res r1 ("A", 5, 7, 10);
        Res r2 ("C", 7, 1, 9);
        Res r3 ("D", 4, 3, 8);
        Res_Table t = {r1, r2};

        t+=r3;
        REQUIRE(t.size() == 3);
        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["C"], r2));
        REQUIRE(equal(t["D"], r3));

        Res r4 {"C", 8, 3, 10};
        Res r5 {"C", 15, 4, 9};
        t+=r4;
        REQUIRE(t.size() == 3);
        REQUIRE(equal(t["C"], r5));

        Res r6 ("B", 2, 6, 4); 
        t+=r6;
        REQUIRE(t.size() == 4);
        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r6));
        REQUIRE(equal(t["C"], r5));
        REQUIRE(equal(t["D"], r3));
    }

    SECTION("DELETE") {
        Res_Table empty; 
        REQUIRE_THROWS(empty.del("B"));  

        Res r1 ("A", 5, 7, 10);
        Res r2 ("C", 7, 1, 9);
        Res r3 ("D", 4, 3, 8);
        Res_Table t = {r1, r2, r3};     
        REQUIRE_THROWS(t.del("B"));
        t.del("C");
        REQUIRE(t.size() == 2);
        REQUIRE_THROWS(t["C"]);
        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["D"], r3));
        t.del("D");
        REQUIRE(t.size() == 1);
        REQUIRE_THROWS(t["D"]);
        REQUIRE(equal(t["A"], r1));
        t.del("A");
        REQUIRE(t.size() == 0);
        REQUIRE_THROWS(t["A"]);
    }
}