#include <initializer_list>
#include <iostream>
#include "lib1/res.h"
#include "lib2/res_table.h"
#include "libgen/gen.h"


int main (void) {

    try {
        SubMenu RES {"MENU"};

        SubMenu RES_GEN =  {"GENERAL", {"Create", "Get", "Set"}};
        SubMenu RES_OTHER = {"OTHER", {"Join", "Compare",
                "Profit", "Turnout"}};

        SubMenu RES_GET = {"GET", {"Name", "Cons", "Prod", "Price"}};
        SubMenu RES_SET = {"SET", {"Name", "Cons", "Prod", "Price"}};

        RES.submenus = {RES_GEN, RES_OTHER, RES_GET, RES_SET};

        std::cout << RES;

        //dialog (RES);
    }

    catch(const std::bad_alloc& ba) { 
        std::cerr << "Not enough memory" << std::endl;
        return 1;
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;   
    }

    return 0;
}

