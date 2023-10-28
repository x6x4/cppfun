/** @file mem/mem.h
 *  
 *  Text and data areas layout.
 */
#pragma once

class SPRegister;
class DataCell;
#include "../IR/IR.h"
#include <cstddef>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <utility>
#include <vector>


using SafeText = std::vector<std::unique_ptr<Command>>;
using Data = std::vector<int>;

using Text = std::vector<Command*>;


//  PROGRAM MEMORY

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Text section with infinite capacity. 
 *  Stores special-purpose program counter register.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ProgramMemory {

friend SPRegister;
friend CPU;

    Text text;
    std::vector<std::size_t> sp_regs = std::vector<std::size_t>(2);
    std::size_t pc_num = 0, zf_num = 1;

    void load (const SafeText &mtext);
    void set_spreg (std::size_t num, std::size_t val);

public:

    ~ProgramMemory(); 
    ProgramMemory () = default;
    ProgramMemory (ProgramMemory&&) = default;
    ProgramMemory &operator= (ProgramMemory &&_pm) = default;
    
    bool is_over ();
    const Command &fetch ();
    void print_regblock(std::ostream &os) const;
    int operator[] (std::size_t num) const { return sp_regs.at(num); }

    friend std::ostream &operator<<(std::ostream &os, ProgramMemory &pm);
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Data section with finite capacity. 
 *  Stores integer data sequantially.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class DataMemory {

friend DataCell;
friend CPU;

    std::size_t sz = 0;
    Data data = Data(sz);
    void load (const Data &mdata);

public:

    DataMemory (std::size_t _sz) : sz (_sz) {};
    friend std::ostream &operator<<(std::ostream &os, DataMemory &pm);
    int operator[] (std::size_t num) const { return data.at(num); }
};


class Memory {
public:
    ProgramMemory pm;
    DataMemory dm;

    Memory (std::size_t sz) : dm(sz) { };
};
