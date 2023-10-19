
#include "cpu.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>



void Mem_Register::load(CPU &cpu) const {
    cpu.get_regblock().regs[num] = value;
}


bool CPU::CPU_exists = 0;

void CPU::check_existence () {
    if (CPU_exists) throw std::logic_error("Can't create more than one CPU");
    CPU_exists = 1;
}

void CPU::exec (const MCode& mc) {
    mem.pm.load(mc);
    const Command &cur_cmd = mem.pm.fetch();
    assign(cur_cmd);
}


void CPU::assign(const Command &cmd) {
    for (std::size_t i = 0; i < EUs.size(); i++) {
        auto &eu = EUs[i];
        if (eu.first == State::FREE) {
            EUs[i].first = State::BUSY;
            std::cout << "Command assigned to EU " << i << std::endl;
            EUs[i].second.exec(cmd);
            return;
        }
    }

    std::cout << "Command can't be assigned now, wait please" << std::endl;
}

void ExecUnit::exec(const Command &cmd, CPU &cpu) const {
    cmd.exec(cpu);
}

