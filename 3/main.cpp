#include "lib/mem/mem.h"
#include <cstring>
#include <iostream>
#include <stdexcept>



int main (int argc, char **argv) {

    const char *src = argv[1];

    if (!src) {
        std::cout << "This is QASM compiler. Usage: qasm <prog_name>" << std::endl;
        return 0;
    }

    Memory m;

    try {
        compile (src, m);
    }

    catch (std::logic_error &e) {
        std::cout << e.what();
    }

    catch (std::runtime_error &e) {
        std::cout << e.what();
    }
}

