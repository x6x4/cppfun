#define CATCH_CONFIG_MAIN

#include <sstream>
#include "../lib1/res.h"
#include <catch2/catch.hpp>


//  SIMPLE CLASS

TEST_CASE("Resource Constructors") {
    SECTION("DefaultConstructor") {
        Res def;
        REQUIRE(def.get_name()  == "(default)");
        REQUIRE(def.get_cons()  == 0);
        REQUIRE(def.get_prod()  == 0);
        REQUIRE(def.get_price() == 0);
    }
    
    SECTION("InitConstructors") {
        Res all ("A", 5, 7, 10);
        REQUIRE(all.get_name()  == "A");
        REQUIRE(all.get_cons()  == 5);
        REQUIRE(all.get_prod()  == 7);
        REQUIRE(all.get_price() == 10);

        Res name_price ("B", 9);
        REQUIRE(name_price.get_name()  == "B");
        REQUIRE(name_price.get_cons()  == 0);
        REQUIRE(name_price.get_prod()  == 0);
        REQUIRE(name_price.get_price() == 9);
    }
    
    SECTION("TestException") {
        REQUIRE_THROWS(Res("C", -5, 3, 4));
        REQUIRE_THROWS(Res("C", 5, -3, 4));
        REQUIRE_THROWS(Res("C", 5, 3, -4));
        REQUIRE_THROWS(Res("C", -4));
    }
}

TEST_CASE("Resource Setters") {
    Res set ("B", 6, 3, 8);

    set.set_name ("A");
    REQUIRE(set.get_name()  == "A");

    set.set_cons (5);
    REQUIRE(set.get_cons()  == 5);
    REQUIRE_THROWS(set.set_cons (-5));

    set.set_prod(7);
    REQUIRE(set.get_prod()  == 7);
    REQUIRE_THROWS(set.set_prod (-7));
    
    set.set_price(10);
    REQUIRE(set.get_price() == 10);
    REQUIRE_THROWS(set.set_price (-10));
}

TEST_CASE("Resource IO") {
    SECTION("INPUT") {
        Res input;
        Res r {"B", 6, 8, 9};
        std::stringstream in("B 6 8 9");
        in >> input;

        REQUIRE(equal (input, r));

        Res input2;
        std::stringstream in2("B -6 8 9");
        REQUIRE_THROWS(in2 >> input2);

        Res input3;
        std::stringstream in3("brrrrrrrrr");
        //std::cout << input3;
        REQUIRE_THROWS(in3 >> input3);     
    }
    SECTION("OUTPUT") {
        Res output ("A", 5, 7, 10);
        std::stringstream out;
        out << output;

        REQUIRE(out.str() == "A 5 7 10");
    }
}

TEST_CASE("Resource Other") {
    SECTION("JOIN") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("A", 2, 6, 4);

        Res s = r1 + r2;
        REQUIRE(s.get_name()  == "A");
        REQUIRE(s.get_cons()  == 7);
        REQUIRE(s.get_prod()  == 13);
        REQUIRE(s.get_price() == 4);
        Res ss = r2 + r1;
        REQUIRE(ss.get_name()  == "A");
        REQUIRE(ss.get_cons()  == 7);
        REQUIRE(ss.get_prod()  == 13);
        REQUIRE(ss.get_price() == 4);

        Res r3 {"B", 7, 2, 5};
        Res s_inval;
        REQUIRE_THROWS(s_inval = r3 + r1);
        REQUIRE_THROWS(s_inval = r1 + r3);
    }
    SECTION("COMPARE") {
        Res r1 ("A", 5, 7, 10);
        Res r2 ("B", 2, 6, 4);
        REQUIRE((r1 <=> r2 < 0));
        REQUIRE((r2 <=> r1 > 0));

        Res r3 {"B", 7, 2, 5};
        REQUIRE((r2 <=> r3 == 0));
    }
    SECTION("PROFIT") {
        Res r1 ("A", 5, 7, 10);
        REQUIRE(r1.get_profit() == 140);

        Res r2 ("B", 6, 2, 4);
        REQUIRE(r2.get_profit() == -112);
    }
    SECTION("MULTIPLY") {
        Res r1 ("A", 5, 7, 10);
        r1 * 7;
        REQUIRE(r1.get_cons() == 35);
        REQUIRE(r1.get_prod() == 49);
    }
}

