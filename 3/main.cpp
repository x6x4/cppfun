#include "lib/prog/prog.h"
#include <iostream>
#include <stdexcept>

int main () {
    ProgramMemory pm;
    

    try {
        load_program ("prog1.asm", pm);
        std::cout << pm;
    }

    catch (std::logic_error &e) {
        std::cout << e.what();
    }

    catch (std::runtime_error &e) {
        std::cout << e.what();
    }
}

