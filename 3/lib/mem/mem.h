#pragma once
#include "../qasm/cmd_base/cmd_base.h"
#include <stdexcept>
#include <utility>
#include <vector>



//  PROGRAM MEMORY

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Text section with infinite capacity. 
 *  Stores special-purpose program counter register.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Op_J;

class ProgramMemory {

friend Op_J;

    std::size_t pc;
    bool zf;

    void set_pc (std::size_t _pc) { pc = _pc; };
    void set_zf (bool _zf)        { zf = _zf; };

    std::size_t get_pc ()         { return pc; };
    bool get_zf ()                { return zf; };

    void swap (ProgramMemory &other) {
        std::swap(this->pc, other.pc);
        std::swap(this->zf, other.zf);
        //std::swap(this->prog, other.prog);
    }

public:

    //  CTORS

    ProgramMemory () {};

    //ProgramMemory(mprog program) { prog = program; } 

    //ProgramMemory(ProgramMemory &_pm) { prog = _pm.prog; }

    ProgramMemory operator= (ProgramMemory &_pm) {
        ProgramMemory pm (_pm);
        swap(pm);
        return *this;
    }
    
    //const Command &get_cur_cmd () const { return *prog[pc]; };
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