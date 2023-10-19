#pragma once

#include "../qasm/mcode_compiler/mcode_compiler.h"
#include "../mem/mem.h"
#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>


///   fwd
class CPU;
class ExecUnit;

using exectime_t = std::size_t; 
using instr_set = std::vector<std::pair<Operator, exectime_t>>;

enum class State {
    FREE, 
    BUSY
};

using ExecUnits = std::vector<std::pair<State,ExecUnit>>;
using num_t = std::size_t;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Contains general-purpose registers. 
 *  Registers can be: 
 *      accessed through number;
 *      blocked.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Mem_Register : public Register {
    void load(CPU &cpu) const;
};

class RegBlock {
friend Mem_Register;

    std::vector<int> regs;

    int operator[] (std::size_t num) const;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Execute command and blocks its operands. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class ExecUnit {

friend CPU;

    State eu_state;

    instr_set set;

    void exec (const Command &cmd, CPU &cpu) const;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  The main device, contains all other devices. 
 *  
 *  It implements fetch-decode-execute cycle: 
 *  fetches next instruction from program memory 
 *  and then assigns it to free execution unit.
 *  Its configuration can be changed. 
 *  
 *  Program memory:  |cmd1|cmd2|cmd3|cmd4|...|   
 *  | cmd | -> EU
 *  Execution units: | cmd3 | | cmd1 | | free | | cmd2 |
 *  
 *  General-purpose register block:  |reg1|reg2|reg3|reg4|...|
 *  Data memory: |||||||||||||||||||||||||
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class CPU {

    std::size_t bitness = 16;

    ExecUnits EUs = {std::make_pair(State::FREE, ExecUnit())};
    RegBlock gp;
    Memory mem;

    InstrSet iset;

    static bool CPU_exists;
    void check_existence(); 

    void assign(const Command &cmd);
    RegBlock &get_regblock () { return gp; }

public:

    CPU(InstrSet& _iset) : iset (_iset) { check_existence(); }
    
    void exec (const MCode &mc);
    
    CPU &edit_config(); 
};

