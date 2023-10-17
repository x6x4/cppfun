
#include "loader.h"

void Mem_Register::load(CPU &cpu) const {
    cpu.get_regblock().regs[num] = value;
}




