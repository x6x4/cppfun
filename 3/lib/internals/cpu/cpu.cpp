
#include "cpu.h"
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <functional>
#include "fmt/core.h"

//  rb

int RegBlock::operator[] (std::size_t num) const { 
    return regs.at(num); 
}

void RegBlock::load_reg (std::size_t num, int val) { regs[num] = val; };

#define YELLOW  "\033[33m"      /* Yellow */
#define RESET   "\033[0m"

void RegBlock::print (std::ostream &os) const { 
    for (std::size_t i = 0; i < regs.size(); i++) 
        os << fmt::format("r{} ", i);
    os << '\n';

    for (std::size_t i = 0; i < regs.size(); i++) {
        if (regs[i]) os << fmt::format(YELLOW" {} " RESET, regs[i]);
        else os << fmt::format(" {} ", regs[i]);
    }
    os << '\n';
}

std::ostream &operator<<(std::ostream &os, RegBlock &rb) {
    rb.print(os);
    return os;
}

//  Exec CPU  //

void exec (CPU &cpu, my_std::Vec <bpNum> &bps, std::function<void(bpNum)> dbg_func) {

    auto bp_num = bps.size() ? bps[0] : bpNum();
    std::size_t count = 0;

    while (!cpu.is_over()) {
        
        if (bp_num.progNum != NO_BPS) {
            std::size_t pc = cpu.get_pc();
            
            //  dbg
            if (pc == bp_num.textNum) {
                dbg_func(bp_num);
                bp_num = (count == bps.size() - 1) ? bpNum() : bps[++count];
            }
        }

        cpu.exec_once();
    }

    dbg_func(bp_num);
    cpu.clear();
}

void CPU::clear() {
    gp_rb.clear();
    mem.pm.clear_regs();
    mem.dm.load(cache_data);
}

//  CPU  //

void CPU::exec_once () {
    const Command &cur_cmd = fetch();
    assign(cur_cmd);
}

void CPU::load_mem (Mem &&m) {
    try {
        mem.dm.load(*m.first);
        mem.pm.load(*m.second);
        cache_data = *m.first;
    }
    catch (std::runtime_error &e) {
        std::cout << __PRETTY_FUNCTION__ << ": " << '\n';
        throw e;
    }
    catch (std::logic_error &e) {
        std::cout << __PRETTY_FUNCTION__ << ": " << '\n';
        throw e;
    }
}

void CPU::assign(const Command &cmd) {
    EU.exec(cmd);
}

//  EXEC UNIT  //

void ExecUnit::exec(const Command &cmd) const {
    cmd.exec_in_cache(cpu->cache);
    cpu->load_from_cache();
}

//  LOAD FROM CACHE  //

void CPU::load_from_cache () {
    for (const auto &opd : cache.opds)
        if (opd) opd->load_to(*this);
    cache.clear();
}
