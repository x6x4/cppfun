
#include "cpu.h"
#include <cstdint>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>


//  DBG

void fill (SafeText &p, void (*oper)()) {
    for (std::size_t i = 0; i < p.size(); i++) {
        p[i]->dbg_oper = oper;
    }
}


//  rb

int RegBlock::operator[] (std::size_t num) const { 
    return regs.at(num); 
    //if (num < regs.size()) 
    //throw std::logic_error("Wrong register number: " + std::to_string(num));
}

void RegBlock::load_reg (std::size_t num, int val) { regs[num] = val; };

void RegBlock::print (std::ostream &os) const { 
    for (std::size_t i = 0; i < regs.size(); i++) 
        if (regs[i]) os << 'r' << i << '(' << regs[i] << ')' << ' '; 
    os << '\n';
}

std::ostream &operator<<(std::ostream &os, RegBlock &rb) {
    rb.print(os);
    return os;
}

//  CPU  //

void CPU::exec (const char *asm_prog, std::vector <std::size_t> &bps) {
    Mem mcode = file_to_mcode(iset, asm_prog, bps);
    load_mem(std::move(mcode));
    exec();
}

void CPU::exec () {

    std::size_t count = 0;

    while (!mem.pm.is_over()) {
        std::size_t pc = mem.pm.get_pc();
        const Command &cur_cmd = mem.pm.fetch();
        assign(cur_cmd);
    }
}


void CPU::load_mem (Mem &&m) {
    mem.dm.load(*m.first);
    mem.pm.load(*m.second);
}

void CPU::assign(const Command &cmd) {
    for (std::size_t i = 0; i < EUs.size(); i++) {
        auto &eu = EUs[i];
        if (eu.first == State::FREE) {
            EUs[i].first = State::BUSY;
            //std::cout << "Command assigned to EU " << i << std::endl;
            EUs[i].second.exec(cmd, *this);
            EUs[i].first = State::FREE;
            return;
        }
    }

    //std::cout << "Command can't be assigned now, wait please" << std::endl;
}

//  EXEC UNIT  //

void ExecUnit::exec(const Command &cmd, CPU &cpu) const {
    cmd.exec(cpu.cache, cpu);
    cpu.load_from_cache();
}

//  LOAD FROM CACHE  //

void CPU::load_from_cache () {
    if (cache.opd1) cache.opd1->load_to(*this);
    if (cache.opd2) cache.opd2->load_to(*this);
    cache.clear();
}
