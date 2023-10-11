
#include "mem.h"
#include <stdexcept>


std::ostream &operator<<(std::ostream &os, ProgramMemory &pm) {
    for (auto cmd : pm.prog) {
        os << *cmd << '\n';
    }
    
    return os;
}