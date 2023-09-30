#define CATCH_CONFIG_MAIN


#include <sstream>
#include "../lib2/res_table.h"
#include <catch2/catch.hpp>

using namespace MY_CLASSES;


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
        Res_Table t = {r3, r1, r2};

        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r2));
        REQUIRE(equal(t["C"], r3));
        REQUIRE(t.size() == 3);
    }
    
    SECTION("CopyConstructor") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r2, r3, r1};
        Res_Table t1 {t};

        REQUIRE(equal(t1["A"], r1));
        REQUIRE(equal(t1["B"], r2));
        REQUIRE(equal(t1["C"], r3));
        REQUIRE(t1.size() == 3);

        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r2));
        REQUIRE(equal(t["C"], r3));
        REQUIRE(t.size() == 3);
    }

    SECTION("Copy-assignment operator") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res r4 ("D", 2, 8, 5);
        Res_Table t = {r2, r3, r1};
        Res_Table tt = {r2, r4, r1, r3};
        Res_Table t2;

        //  0 <- 3
        t2 = t;
        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r2));
        REQUIRE(equal(t["C"], r3));
        REQUIRE(t.size() == 3);

        REQUIRE(equal(t2["A"], r1));
        REQUIRE(equal(t2["B"], r2));
        REQUIRE(equal(t2["C"], r3));
        REQUIRE(t2.size() == 3);

        REQUIRE_NOTHROW(t2 = t2);
        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r2));
        REQUIRE(equal(t["C"], r3));
        REQUIRE(t2.size() == 3); 

        //  3 <- 3
        t2 = t;
        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r2));
        REQUIRE(equal(t["C"], r3));
        REQUIRE(t.size() == 3);
        
        REQUIRE(equal(t2["A"], r1));
        REQUIRE(equal(t2["B"], r2));
        REQUIRE(equal(t2["C"], r3));
        REQUIRE(t2.size() == 3);  

        //  3 <- 4
        t = tt;
        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r2));
        REQUIRE(equal(t["C"], r3));
        REQUIRE(equal(t["D"], r4));
        REQUIRE(t.size() == 4);

        REQUIRE(equal(tt["A"], r1));
        REQUIRE(equal(tt["B"], r2));
        REQUIRE(equal(tt["C"], r3));
        REQUIRE(equal(tt["D"], r4));
        REQUIRE(tt.size() == 4);

        //  4 <- 3
        tt = t2;
        REQUIRE(equal(tt["A"], r1));
        REQUIRE(equal(tt["B"], r2));
        REQUIRE(equal(tt["C"], r3));
        REQUIRE(tt.size() == 3);

        REQUIRE(equal(t2["A"], r1));
        REQUIRE(equal(t2["B"], r2));
        REQUIRE(equal(t2["C"], r3));
        REQUIRE(t2.size() == 3);      
    }
    
    SECTION("MoveConstructor") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r2, r3, r1};
        Res_Table tt {t};
        
        Res_Table t1 {std::move(t)};

        REQUIRE(equal(t1["A"], r1));
        REQUIRE(equal(t1["B"], r2));
        REQUIRE(equal(t1["C"], r3));
        REQUIRE(t1.size() == 3);
        
        REQUIRE(t.size() == 0);
    }

    SECTION("Move-assignment operator") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        Res r3 ("C", 4, 3, 8);
        Res_Table t = {r3, r2, r1};
        Res_Table tt {t};

        Res_Table t2 = {r3, r1};
        //  tt 3, t2 2, t 3
        //  2 -> 3

        tt = std::move(t2);
        //  tt 2, t2 0, t 3
        REQUIRE(equal(tt["A"], r1));
        REQUIRE(equal(tt["C"], r3));
        REQUIRE(tt.size() == 2);
        REQUIRE(t2.size() == 0);
        //  3 -> 0

        t2 = std::move(t);
        //  tt 2, t2 3, t 0
        REQUIRE(equal(t2["A"], r1));
        REQUIRE(equal(t2["B"], r2));
        REQUIRE(equal(t2["C"], r3));
        REQUIRE(t2.size() == 3);
        REQUIRE(t.size() == 0);
        //  3 -> 2

        tt = std::move(t2);
        //  tt 3, t2 0, t 0
        REQUIRE(equal(tt["A"], r1));
        REQUIRE(equal(tt["B"], r2));
        REQUIRE(equal(tt["C"], r3));
        REQUIRE(tt.size() == 3);
        REQUIRE(t2.size() == 0);   
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

        std::stringstream out2;
        Res_Table a;
        out2 << a;
        REQUIRE(out2.str() == "Empty table");
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

        const Res_Table tt = {r2, r3};
        REQUIRE(equal(tt["B"], r2));
        REQUIRE(equal(tt["D"], r3));
        REQUIRE_THROWS(tt["a"]);
        REQUIRE_THROWS(tt["C"]);
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
        REQUIRE(t["A"].get_cons() == 35);
        REQUIRE(t["A"].get_prod() == 49);
        REQUIRE(t["B"].get_cons() == 14);
        REQUIRE(t["B"].get_prod() == 42);
        REQUIRE(t["C"].get_cons() == 28);
        REQUIRE(t["C"].get_prod() == 21);
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
        REQUIRE_THROWS(t+=r4);
        //REQUIRE(t.size() == 3);
        //REQUIRE(equal(t["C"], r5));

        /*Res r6 ("B", 2, 6, 4); 
        t+=r6;
        REQUIRE(t.size() == 4);
        REQUIRE(equal(t["A"], r1));
        REQUIRE(equal(t["B"], r6));
        REQUIRE(equal(t["C"], r5));
        REQUIRE(equal(t["D"], r3));*/
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