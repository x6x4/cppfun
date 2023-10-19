#pragma once
#include "../qasm/mcode_compiler/mcode_compiler.h"
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

///  fwd
class Op_J;

///  aliases
using pc_reg = std::size_t;
using flag_reg = bool;


//  PROGRAM MEMORY

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Text section with infinite capacity. 
 *  Stores special-purpose program counter register.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


class ProgramMemory {

friend Op_J;
friend CPU;

    MCode text;
    pc_reg pc = 0;
    flag_reg zf = 0;

///  GENERAL 

    void load (const MCode &mcode) { text = mcode; }

///  GET & SET

    pc_reg get_pc ()   { return pc; };
    flag_reg get_zf () { return zf; };

    void set_pc (pc_reg _pc)   { pc = _pc; };
    void set_zf (flag_reg _zf) { zf = _zf; };

///  OTHER

    void swap (ProgramMemory &other) {
        std::swap(this->pc, other.pc);
        std::swap(this->zf, other.zf);
        //std::swap(this->prog, other.prog);
    }

public:

    ~ProgramMemory() = default;

    //  CTORS

    ProgramMemory () = default;
    ProgramMemory (ProgramMemory&&) = default;

    ProgramMemory &operator= (ProgramMemory &&_pm) = default;
    
    const Command &fetch () { return *text[pc]; pc++; }
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