
#include "cpu.h"
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stdexcept>


bool CPU::CPU_exists = 0;

void CPU::check_existence () {
    if (CPU_exists) throw std::logic_error("Can't create more than one CPU");
    CPU_exists = 1;
}

//  class CPU

/*CPU::CPU(EU_vec _eu, RegBlock &_gp, Memory &_m)
    : EUs(_eu), gp(_gp), m(_m) {
        
        std::ofstream os ("CPU.conf");
        std::string conf = "Program Memory:\nControl Unit:\nExecution Units:\nRegisters:\nData Memory:\n";
        
        if (os.is_open()) os << conf;
        if (os.bad()) throw std::runtime_error ("Error creating proc.conf");
        //  export config
};   */


CPU &CPU::edit_config () {
    system("vim CPU.conf");
    //  import config
    return *this;
}

//  class ProgramMemory

/*ProgramMemory::ProgramMemory(prog_t prog, DataMemory &dm) : prog(prog), PC(pc) {};

const Command &ProgramMemory::get_cur_cmd() const {
    return prog[*PC.second];
}*/

//  class RegBlock


//  class ControlUnit

void ControlUnit::operator() (CPU &cpu) {
    const Command &cur_cmd = fetch(cpu);
    assign(cpu, cur_cmd);
}

const Command &ControlUnit::fetch (CPU &cpu) const {
    return cpu.m.pm.get_cur_cmd();
}

enum {
    FREE, 
    BUSY
};


void ControlUnit::assign(CPU &cpu, const Command &cmd) {
    for (std::size_t i = 0; i < cpu.EUs.size(); i++) {
        if (cpu.EUs[i].first == FREE) {
            cpu.EUs[i].first = BUSY;
            std::cout << "Command assigned to EU " << i << std::endl;
            cpu.EUs[i].second.exec(cmd);
            break;
        }
    }

    std::cout << "Command can't be assigned now, wait please" << std::endl;
}

void ExecUnit::exec(const Command &cmd) const {
    
}

