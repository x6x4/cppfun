
#include "mem.h"
#include <cstddef>
#include <memory>
#include <sstream>

//  pm

ProgramMemory::~ProgramMemory() {
    for (auto cmd : text) delete cmd;
}

void ProgramMemory::clear() {
    clear_regs();

    for (auto cmd : text) delete cmd;
    text.clear();
}

void ProgramMemory::load (const SafeText &mtext) { 
    
    clear();

    for (std::size_t i = 0; i < mtext.size(); i++) {
        text.push_back((mtext[i]->clone()));
    }
}

void ProgramMemory::set_spreg (std::size_t num, std::size_t val) { 
    if (num < text.size()) sp_regs[num] = val; 
    else throw std::logic_error("Access out of bounds");
}

bool ProgramMemory::is_over () { return sp_regs[pc_num] == text.size(); }

const Command &ProgramMemory::fetch () { return *text[sp_regs[pc_num]++]; }

void ProgramMemory::print_regblock(std::ostream &os) const{
    os << "pc(" << sp_regs[pc_num] << ")" << ' '; 
    os << "zf(" << sp_regs[zf_num] << ")" << ' ';
    os << '\n';
}

std::ostream &operator<<(std::ostream &os, ProgramMemory &pm) {
    for (std::size_t i = 0; i < pm.text.size(); i++)
        os << *pm.text[i] << '\n';
    return os;
}

//  dm

void DataMemory::load (const Data &mdata) { 
    
    for (std::size_t i = 0; i < mdata.size(); i++) {
        data.at(i) = mdata.at(i); 
    }
}

std::ostream &operator<<(std::ostream &os, DataMemory &dm) {
    for (auto cell : dm.data) os << cell << ' ';
    os << '\n';
    return os;
}