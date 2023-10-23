#pragma once

#include "fwd_mem.h"


class Directive;

template<typename T>

class MCode {

    std::vector<T*> lines;

public:

    ~MCode ();
    
    MCode () = default;
    MCode (MCode&) = default;
    MCode (const MCode&) = default;
    MCode (MCode&&) = default;
    MCode &operator= (const MCode& _mc);
    MCode& operator= (MCode&&) = default;

    std::size_t num_lines ();
    T* operator[] (std::size_t num) const;

    void add (T* cmd);
    void print_mcode (std::ostream &os) const;

};

template<typename T>
std::ostream &operator<<(std::ostream &os, const MCode<T> &mc);

template<typename T>
MCode<T> file_to_mcode (InstrSet &iset, const char *filename);


//  PROGRAM MEMORY

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Text section with infinite capacity. 
 *  Stores special-purpose program counter register.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class ProgramMemory {

friend SPRegister;
friend CPU;

    MCode<Command> text;
    std::vector<std::size_t> sp_regs = std::vector<std::size_t>(2);
    std::size_t pc_num = 0, zf_num = 1;

    void load (const MCode<Command> &mtext);
    void set_spreg (std::size_t num, std::size_t val);

public:

    ~ProgramMemory() = default; 
    ProgramMemory () = default;
    ProgramMemory (ProgramMemory&&) = default;
    ProgramMemory &operator= (ProgramMemory &&_pm) = default;
    
    bool is_over ();
    const Command &fetch ();
    void print_regblock(std::ostream &os) const;

    friend std::ostream &operator<<(std::ostream &os, ProgramMemory &pm);
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Data section with finite capacity. 
 *  Stores integer data sequantially.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class DataMemory {
    
    std::size_t sz = 0;
    std::vector<std::pair<std::string, int>> mem = std::vector<std::pair<std::string, int>>(sz);

public:

    DataMemory (std::size_t _sz) : sz (_sz) {};

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

class Data_labels {
public:
    std::size_t cur_cmd = 0;
    std::vector<std::pair<std::size_t, std::string>> has_data;
};

class DataCell {

    ID name;
    int data;

public:
    DataCell (std::string name, int data) : data(data) {
        this->name = name;
    };   
    int operator* () { return data;}
};


class Memory {
public:
    ProgramMemory pm;
    DataMemory dm;

    Memory (std::size_t sz) : dm(sz) { };
};
