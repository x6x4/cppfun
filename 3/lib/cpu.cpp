
#include "cpu.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>


//  class CPU

CPU::CPU(ProgramMemory &_pm, EU_vec _EUs, RegBlock &_gp, DataMemory &_dm) 
    : pm(_pm), EUs(_EUs), gp(_gp), dm(_dm) {
        
        std::ofstream os ("CPU.conf");
        std::string conf = "Program Memory:\nControl Unit:\nExecution Units:\nRegisters:\nData Memory:\n";
        
        if (os.is_open()) os << conf;
        if (os.bad()) throw std::runtime_error ("Error creating proc.conf");
        //  export config
    };   

void CPU::run () {
    cu();
}

CPU &CPU::edit_config () {
    system("vim CPU.conf");
    //  import config
    return *this;
}

//  class ProgramMemory

ProgramMemory::ProgramMemory(prog_t prog, DataMemory &dm) : prog(prog), PC(pc) {};

const Command &ProgramMemory::get_cur_cmd() const {
    return prog[*PC.second];
}

//  class RegBlock


//  class ControlUnit

void ControlUnit::operator() () {
    const Command &cur_cmd = fetch();
    assign(cur_cmd);
}

const Command &ControlUnit::fetch () const {
    return pm.get_cur_cmd();
}

enum {
    FREE, 
    BUSY
};

void ControlUnit::assign(const Command &cmd) {
    for (std::size_t i; i < EUs.size(); i++) {
        if (EUs[i].first == FREE) {
            EUs[i].first = BUSY;
            std::cout << "Command assigned to EU " << i << std::endl;
            EUs[i].second.exec(cmd);
            break;
        }
    }

    std::cout << "Command can't be assigned now, wait please" << std::endl;
}

void ExecUnit::exec(const Command &cmd) const {
    
}

