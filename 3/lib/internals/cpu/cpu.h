/** @file cpu/cpu.h
 *  
 *  Arch-dependent operands and devices.
 */
#pragma once

class CPU;
class ExecUnit;
#include "../mem/mem.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>


using exectime_t = std::size_t; 

enum class State {
    FREE, 
    BUSY
};

using ExecUnits = std::vector<std::pair<State,ExecUnit>>;

using Mem = std::pair<std::unique_ptr<Data>, std::unique_ptr<SafeText>>;

Mem file_to_mcode (InstrSet &iset, const char *filename);


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
    ~GPRegister () override = default;
    std::unique_ptr<Operand> clone () const override;

    GPRegister(std::size_t number) : num(number) {};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Small and fast memory cell inside Program Memory. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class SPRegister : public GPRegister {

protected:

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    SPRegister () {};
    ~SPRegister () override = default;
    std::unique_ptr<Operand> clone () const override;

    SPRegister (std::size_t _val) { value = _val; }
    void set_num (std::size_t _num) { num = _num; }  
};


class DataCell : public GPRegister {
protected:

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    DataCell () {};
    ~DataCell () override = default;
    std::unique_ptr<Operand> clone () const override;

    DataCell (std::size_t _val) { value = _val; }
    void set_num (std::size_t _num) { num = _num; }  
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

    CPU(InstrSet& _iset) : iset (_iset) { }

    std::size_t get_bitness () { return bitness; }
    int gp (std::size_t num) const { return gp_rb[num]; }
    int sp (std::size_t num) const { return mem.pm[num]; }
    int data (std::size_t num) const { return mem.dm[num]; }
    
    void exec (const char *asm_prog);
    void exec_mcode (Mem &&m);
    void print_regblock (std::ostream &os) { os << gp_rb; }
};

