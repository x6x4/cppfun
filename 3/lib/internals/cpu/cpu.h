/** @file cpu/cpu.h
 *  
 *  CPU and its internal devices.
 */
#pragma once
#include "fwd_cpu.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <functional>

/**
 * @class RegBlock
 * @brief Contains 16 general-purpose registers.  
 *
 * Registers can be: 
 *      accessed through number;
 *      blocked.
 */
class RegBlock {
friend NumberedCell;
friend CPU;

    std::size_t num_reg = 0;
    my_std::Vec<int> regs = my_std::Vec<int>(num_reg);

    void load_reg (std::size_t num, int val);
    int operator[] (std::size_t num) const;

public:
    
    /**
    * @brief Prints contents of register block.
    * @param os The output stream for printing.
    */
    void print (std::ostream &os) const;

    /**
    * @brief Clears contents of register block.
    */  
    void clear () { for (auto &reg : regs) reg = 0; }

    /**
    * @brief Constructor for the RegBlock class.
    *
    * @param num_reg The number of registers in block.
    */
    RegBlock(std::size_t _num_reg) : num_reg(_num_reg) { }; 
};

/**
* @brief Overloaded output operator to stream the RegBlock object.
*
* @param os The output stream where the RegBlock object will be streamed.
* @param rb The RegBlock object to be streamed.
* @return A reference to the output stream after streaming the RegBlock object.
*/
std::ostream &operator<<(std::ostream &os, RegBlock &rb);


/**
 * @class ExecUnit
 * @brief Private element of CPU, execute command and blocks its operands.
 */
class ExecUnit {
friend CPU;

    CPU *cpu;
    ExecUnit (CPU *_cpu) : cpu(_cpu) {};

    State eu_state;

    void exec (const Command &cmd) const;
};


/**
 * @class CPU
 * @brief The main device, contains all other devices.  
 *
 *  It fetches next instruction from program memory 
 *  and then assigns it to free execution unit.
 */
class CPU {
friend PCRegister;
friend NumberedCell;
friend String;
friend DataCell;
friend ExecUnit;

    std::size_t bitness = 4;
    std::size_t mem_cap = std::pow(2, bitness);

    ExecUnit EU = ExecUnit (this);
    RegBlock gp_rb = RegBlock(8);
    Memory mem = Memory(mem_cap);
    Data cache_data;
    CPU_Cache cache = CPU_Cache(this);

    InstrSet iset;

    void assign(const Command &cmd);
    void load_from_cache ();
    const Command &fetch () { return mem.pm.fetch(); }

public:

    CPU (const CPU &cpu) = delete;
    CPU (CPU &cpu) = delete;

    /**
    * @brief Constructor for the CPU class.
    *
    * @param _iset Instruction set.
    */
    CPU(InstrSet &&_iset) : iset(_iset) { }

        /**
    * @brief Constructor for the CPU class.
    *
    * @param _iset Instruction set.
    */
    CPU(InstrSet &&_iset, std::size_t _bitness) : bitness(_bitness), iset(_iset) { }

    /**
    * @brief Accessor for general-purpose register block by number.
    *
    * @return The value of general-purpose register.
    */
    int gp (std::size_t num) const { return gp_rb[num]; }

    /**
    * @brief Accessor for special-purpose register block by number.
    *
    * @return The value of special-purpose register.
    */
    int sp (std::size_t num) const { return mem.pm[num]; }

    /**
    * @brief Accessor for data memory.
    *
    * @return The ref to data.
    */
    auto &data() const { return mem.dm.data; }

    /**
    * @brief Trivial accessor for instruction set.
    *
    * @return The const reference to instruction set.
    */
    const auto &iSet () const { return iset; }

    /**
    * @brief Indicates if execution of program ended.
    *
    * @return True if program ended execution, false otherwise.
    */
    bool is_over () { return mem.pm.is_over(); }
    
    /**
    * @brief Trivial accessor for program counter.
    *
    * @return The value of program counter.
    */
    std::size_t get_pc () { return mem.pm.get_pc(); }

    /**
    * @brief Load given memory to own memory.
    * 
    * @param m Given memory
    */  
    void load_mem (Mem &&m);

    /**
    * @brief Load own memory using program file.
    *
    * @param filename Program file
    */  
    void load_mem (const char *filename);

    /**
    * @brief Execute current command.
    */  
    void exec_once ();

    /**
    * @brief Clears contents of general-purpose register block.
    */  
    void clear();
    
    /**
    * @brief Prints contents of general-purpose register block.
    * @param os The output stream for printing.
    */
    void print_gpregblock (std::ostream &os) { os << gp_rb; }

    /**
    * @brief Prints contents of special-purpose register block.
    * @param os The output stream for printing.
    */
    void print_spregblock (std::ostream &os) { mem.pm.print_regblock(os); }

    /**
    * @brief Prints contents of data memory block.
    * @param os The output stream for printing.
    */
    void print_dm (std::ostream &os) { os << mem.dm; }

    /**
    * @brief Prints contents of program memory block.
    * @param os The output stream for printing.
    */
    void print_pm (std::ostream &os) { os << mem.pm; }
};

#define NO_BPS SIZE_MAX

struct bpNum {
    std::size_t textNum = NO_BPS;
    std::size_t progNum = NO_BPS;

    bpNum() {};
    bpNum(std::size_t num1, std::size_t num2) : textNum(num1), progNum(num2) {};
};

/**
 * @brief Executes the CPU using breakpoints and debug function.
 *
 * @param cpu The CPU object to execute.
 * @param bps The vector of breakpoints.
 * @param dbg_func The callback function for debugging.
 */
void exec(CPU &cpu, my_std::Vec<bpNum>& bps, std::function<void(bpNum)> dbg_func);
