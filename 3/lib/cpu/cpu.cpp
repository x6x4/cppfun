
#include "cpu.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>

//  REG  //

void SPRegister::load_to(CPU &cpu) const {
    cpu.mem.pm.sp_regs[num] = value;
}

void SPRegister::load_from(CPU &cpu) {}

void SPRegister::print (std::ostream &os) const {
    os << '(' << val() << ')';
}

void GPRegister::load_to(CPU &cpu) const {
    cpu.gp_rb.load_reg(num, value);
}

void GPRegister::load_from(CPU &cpu) {
    value = cpu.gp_rb[num];
}

void GPRegister::print (std::ostream &os) const {
    os << "%r" << num << '(' << val() << ')';
}

std::ostream &operator<<(std::ostream &os, RegBlock &rb) {
    rb.print(os);
    return os;
}

//  CPU  //

bool CPU::CPU_exists = 0;

void CPU::check_existence () {
    if (CPU_exists) throw std::logic_error("Can't create more than one CPU");
    CPU_exists = 1;
}

void CPU::exec (const MCode& mc) {
    mem.pm.load(mc);
    while (!mem.pm.is_over()) {
        const Command &cur_cmd = mem.pm.fetch();
        assign(cur_cmd);
    }
}

void CPU::assign(const Command &cmd) {
    for (std::size_t i = 0; i < EUs.size(); i++) {
        auto &eu = EUs[i];
        if (eu.first == State::FREE) {
            EUs[i].first = State::BUSY;
            std::cout << "Command assigned to EU " << i << std::endl;
            EUs[i].second.exec(cmd, *this);
            EUs[i].first = State::FREE;
            return;
        }
    }

    std::cout << "Command can't be assigned now, wait please" << std::endl;
}

//  EXEC UNIT  //

void ExecUnit::exec(const Command &cmd, CPU &cpu) const {
    cmd.exec(cpu.cache, cpu);
    cpu.load_from_cache();
    cpu.print_regblock(std::cout);
}

//  LOAD FROM CACHE  //

void CPU::load_from_cache () {
    if (cache.opd1) cache.opd1->load_to(*this);
    if (cache.opd2) cache.opd2->load_to(*this);
    cache.clear();
}
