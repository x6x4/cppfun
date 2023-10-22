#pragma once

#include "../qasm/mcode_compiler/mcode_compiler.h"
#include "../mem/mem.h"
#include <algorithm>
#include <cstddef>
#include <memory>
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





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Small and fast memory cell inside CPU. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class GPRegister : public Operand {

protected:
    std::size_t num = 0;

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    GPRegister() {};
    std::unique_ptr<Operand> clone () const override { return std::make_unique<GPRegister>(*this); }
    ~GPRegister () override = default;

    GPRegister(std::size_t number) : num(number) {};
};

class SPRegister : public GPRegister {

protected:

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    std::unique_ptr<Operand> clone () const override { return std::make_unique<SPRegister>(*this); }
    SPRegister () {};
    SPRegister (std::size_t _val) { value = _val; };
    void set_num (std::size_t _num) { num = _num; }
    ~SPRegister () override = default;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Contains 16 general-purpose registers. 
 *  Registers can be: 
 *      accessed through number;
 *      blocked.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


class RegBlock {
friend GPRegister;

    std::vector<int> regs = std::vector<int>(16);

    int operator[] (std::size_t num) const { return regs[num]; }

    void load_reg (std::size_t num, int val) { regs[num] = val; }

public:

    void print (std::ostream &os) const { 
        for (std::size_t i = 0; i < regs.size(); i++) 
            if (regs[i]) os << 'r' << i << '(' << regs[i] << ')' << ' '; 
    }
};

std::ostream &operator<<(std::ostream &os, RegBlock &rb);

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
friend SPRegister;
friend GPRegister;
friend ExecUnit;

    std::size_t bitness = 16;

    ExecUnits EUs = {std::make_pair(State::FREE, ExecUnit())};
    RegBlock gp_rb;
    Memory mem;
    Cache cache;

    InstrSet iset;

    static bool CPU_exists;
    void check_existence(); 

    void assign(const Command &cmd);
    void load_from_cache ();

public:

    CPU(InstrSet& _iset) : iset (_iset) { check_existence(); }
    
    void exec (const MCode &mc);

    void print_regblock (std::ostream &os) { os << gp_rb; os << '\n'; }

    CPU &edit_config(); 
};

