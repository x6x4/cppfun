#pragma once
#include "../qasm/mcode_compiler/mcode_compiler.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

///  fwd
class SPRegister;

///  aliases



//  PROGRAM MEMORY

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Text section with infinite capacity. 
 *  Stores special-purpose program counter register.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


class ProgramMemory {

friend SPRegister;
friend CPU;

    MCode text;
    std::vector<std::size_t> sp_regs = std::vector<std::size_t>(2);
    std::size_t pc_num = 0;
    std::size_t zf_num = 1;

///  GENERAL 

    void load (const MCode &mcode) { text = mcode; }

///  GET & SET

    std::size_t get_pc ()   { return sp_regs[pc_num]; }
    bool get_zf () { return sp_regs[zf_num]; }

    void set_pc (std::size_t _pc)   { sp_regs[pc_num] = _pc; };
    void set_zf (bool _zf) { sp_regs[zf_num] = _zf; };

///  OTHER


public:

    ~ProgramMemory() = default;

    //  CTORS

    ProgramMemory () = default;
    ProgramMemory (ProgramMemory&&) = default;

    ProgramMemory &operator= (ProgramMemory &&_pm) = default;
    
    bool is_over () { return sp_regs[pc_num] == text.num_lines(); }
    const Command &fetch () { return *text[sp_regs[pc_num]++]; }
};

std::ostream &operator<<(std::ostream &os, ProgramMemory &pm);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Data section with finite capacity. 
 *  Stores integer data sequantially.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class DataMemory {
    
    std::size_t sz = 0x10;
    std::vector<std::pair<std::string, int>> mem;

public:

    std::pair <bool, std::size_t> operator[] (std::string name) {

        for (std::size_t i = 0; i < mem.size(); i++) {
            if (!mem[i].first.compare(name)) return {1, i};
        }
        
        return {0, 0};
    }

    DataMemory &operator+=(std::pair<std::string, int> p) {
        if ((*this)[p.first].first)
            throw std::runtime_error ("This label already exists.\n");
        else 
            mem.push_back({p});

        return *this;
    }
};


class Memory {
public:
    ProgramMemory pm;
    DataMemory dm;

    Memory () {};
};

class Data_labels {
public:
    std::size_t cur_cmd = 0;
    std::vector<std::pair<std::size_t, std::string>> has_data;
};

void compile (const char *filename, Memory &m);

class DataCell {

    ID name;
    int data;

public:
    DataCell (std::string name, int data) : data(data) {
        this->name = name;
    };   
    int operator* () { return data;}
};