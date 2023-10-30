
#include "cpu.h"
#include <cstdint>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>


//  rb

int RegBlock::operator[] (std::size_t num) const { 
    return regs.at(num); 
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

void CPU::exec (std::vector <std::size_t> &bps, void (*dbg)() ) {

    std::size_t bp_num = bps.size() ? bps[0] : SIZE_MAX;
    std::size_t count = 0;

    while (!mem.pm.is_over()) {
        std::size_t pc = mem.pm.get_pc();
        
        //  dbg
        if (pc == bp_num) {
            bp_num = (count == bps.size() - 1) ? SIZE_MAX : bps[++count];
            dbg();
        }

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
