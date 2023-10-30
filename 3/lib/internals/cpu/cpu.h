/** @file cpu/cpu.h
 *  
 *  CPU and its internal devices.
 */
#pragma once
#include "fwd_cpu.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Contains 16 general-purpose registers. 
 *  Registers can be: 
 *      accessed through number;
 *      blocked.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class RegBlock {
friend GPRegister;

    std::size_t num_reg = 0;
    std::vector<int> regs = std::vector<int>(num_reg);

    void load_reg (std::size_t num, int val);

public:
    int operator[] (std::size_t num) const;
    void print (std::ostream &os) const;
    RegBlock(std::size_t _num_reg) : num_reg(_num_reg) {}; 
};

std::ostream &operator<<(std::ostream &os, RegBlock &rb);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Execute command and blocks its operands. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ExecUnit {

friend CPU;

    State eu_state;

    void exec (const Command &cmd, CPU &cpu) const;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  The main device, contains all other devices. 
 *  
 *  It fetches next instruction from program memory 
 *  and then assigns it to free execution unit.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class CPU {
friend SPRegister;
friend GPRegister;
friend DataCell;
friend ExecUnit;

    std::size_t bitness = 8;

    ExecUnits EUs = {std::make_pair(State::FREE, ExecUnit())};
    RegBlock gp_rb = RegBlock(bitness);
    Memory mem = Memory(bitness);
    Cache cache;

    InstrSet iset;

    void assign(const Command &cmd);
    void load_from_cache ();

public:

    CPU (const CPU &cpu) = default;
    CPU (CPU &cpu) = default;
    CPU(InstrSet& _iset) : iset (_iset) { }

    int gp (std::size_t num) const { return gp_rb[num]; }
    int sp (std::size_t num) const { return mem.pm[num]; }
    int data (std::size_t num) const { return mem.dm[num]; }
    std::size_t data_sz () const { return mem.dm.sz; }
    
    void load_mem (Mem &&m);
    void exec (std::vector <std::size_t> &bps, void (*dbg)() );

    void print_regblock (std::ostream &os) { os << gp_rb; }
    void print_dm (std::ostream &os) { os << mem.dm; }
    void print_pm (std::ostream &os) { os << mem.pm; }
};

