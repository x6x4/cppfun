#pragma once
#include "../prog/prog.h"
#include <stdexcept>
#include <utility>
#include <vector>


//  PROGRAM MEMORY

using prog_t = std::vector<Command>;
class ProgramMemory;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Text section with infinite capacity. 
 *  Stores special-purpose program counter register.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class ProgramMemory {
    
    //reg &PC;
    addr_t pc;

public:

    prog_t prog = {};
    
    const Command &get_cur_cmd () const;
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