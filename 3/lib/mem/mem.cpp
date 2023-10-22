
#include "mem.h"

//  pm

void ProgramMemory::load (const MCode &mcode) { text = mcode; }

void ProgramMemory::set_spreg (std::size_t num, std::size_t val) { 
    if (num < text.num_lines()) sp_regs[num] = val; 
    else throw std::logic_error("Access out of bounds");
}

bool ProgramMemory::is_over () { return sp_regs[pc_num] == text.num_lines(); }

const Command &ProgramMemory::fetch () { return *text[sp_regs[pc_num]++]; }

void ProgramMemory::print_regblock(std::ostream &os) const{
    os << "pc (" << sp_regs[pc_num] << ")" << '\n'; 
    os << "zf (" << sp_regs[zf_num] << ")" << '\n';
    os << '\n';
}

std::ostream &operator<<(std::ostream &os, ProgramMemory &pm) {
    os << pm.text;
    return os;
}