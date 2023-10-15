
#include "mem.h"
#include <stdexcept>


std::ostream &operator<<(std::ostream &os, ProgramMemory &pm) {

    print_mprog(os, pm.prog);
    
    return os;
}