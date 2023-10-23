
#include "mem.h"


//  MCode

template<typename T>
MCode<T>::~MCode<T> () {
    for (auto ln : lines) { delete ln; ln = nullptr; }
    lines.clear();
}

template<typename T>
MCode<T> &MCode<T>::operator= (const MCode<T>& _mc) {
    lines = _mc.lines;
    for (std::size_t i = 0; i < _mc.lines.size(); i++) 
        lines[i] = _mc.lines[i]->clone();

    return *this;
}

template<typename T>
std::size_t MCode<T>::num_lines () { return lines.size(); }

template<typename T>
T *MCode<T>::operator[] (std::size_t num) const { 
    if (num < lines.size()) return lines[num]; 
    throw std::logic_error("Access out of bounds");
}

template<typename T>
void MCode<T>::add (T* ln) { lines.push_back(ln); };

template<typename T>
void MCode<T>::print_mcode (std::ostream &os) const {
    for (auto ln : lines) os << *ln << '\n';
}

template<typename T>
std::ostream &operator<<(std::ostream &os, const MCode<T> &mc) {
    mc.print_mcode(os);
    return os;
}

//  pm

void ProgramMemory::load (const MCode<Command> &mcode) { text = mcode; }

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