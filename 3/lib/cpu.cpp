
#include "cpu.h"
#include <cstddef>
#include <iostream>


//  class CPU

void CPU::run () {
    cu.run();
}


//  class ProgramMemory

const Command &ProgramMemory::get_cur_cmd() const {
    return prog[*PC->second];
}

//  class RegBlock


//  class ControlUnit

void ControlUnit::run() {
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

void check_id (std::string name) {
    if (name.size() > 8)
        throw std::logic_error ("ID can't be more than 8 symbols");
    
    for (char c : name) {
        if (!std::isalpha(c))
            throw std::logic_error ("ID can only contain latin letters");
    }
}