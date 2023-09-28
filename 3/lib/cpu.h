#pragma once

#include "vec.h"
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <utility>
#include <vector>


class Command;
class ProgramMemory;

class ControlUnit;

class Operator;
using exectime_t = std::size_t; 
using instr_set = std::vector<std::pair<Operator, exectime_t>>;
class ExecUnit;

class DataCell;
class DataMemory;

using num_t = std::size_t;
using reg = std::pair<num_t, DataCell>;
class RegBlock;


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  The main device, contains all other devices. 
 *  It can execute program, transfering control to control unit.
 *  Its configuration can be changed. 
 *  
 *  Program memory:  |cmd1|cmd2|cmd3|cmd4|...|
 *  Control unit:    | cmd | -> EU
 *  Execution units: | cmd3 | | cmd1 | | free | | cmd2 |
 *  
 *  General-purpose register block:  |reg1|reg2|reg3|reg4|...|
 *  Data memory: |||||||||||||||||||||||||
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class CPU {
friend ControlUnit;

    

//  no pointers
    ProgramMemory &pm;
    ControlUnit &cu;
    std::vector<std::pair<bool,ExecUnit>> EUs;
    RegBlock &gp;
    DataMemory &dm;

    void run ();
//    CPU &edit_config(); 
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Code section with infinite capacity. 
 *  Stores special-purpose program counter register.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class ProgramMemory {
friend ControlUnit;

    std::vector<Command> prog;
    reg *PC;

public:
    const Command &get_cur_cmd () const;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Contains general-purpose registers. 
 *  Registers can be: 
 *      accessed through number;
 *      blocked.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class RegBlock {

    Vector<reg> regs;

    int operator[] (std::size_t num) const;
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Implement read-fetch-execute cycle. 
 * It fetches next instruction from program memory 
 * and then assigns it to free execution unit.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class ControlUnit : CPU {
friend CPU;
    
    void run();
    const Command &fetch() const;
    //  changes exec units state
    void assign(const Command &cmd);

};



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Execute command and blocks its operands. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class ExecUnit {

friend ControlUnit;

    instr_set set;
    void exec (const Command &cmd) const;
    
};




void check_id (std::string name);

class ID {
    std::string id;
    
public:
    void operator=(std::string name) {
        check_id (name);
        id = name;
    } 
};

class Operand {

};

class Operator {
    ID label;

public:

    void operator() () const;
};


class Command {

    Operand op1;
    Operator *op;
    ID label;

public:

    Operator *get_op() const { return op; }

};

class UnaryCommand : public Command {

};

class BinaryCommand : public Command {
    Operand op2;

};



class Op_Jump : Operator {
    void operator() () const { };
};

class Op_Data : Operator{
    Operand num;
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

class DataMemory {
    
    std::size_t sz = 0x10;
    std::unordered_set<DataCell> mem;

    int operator[] (std::string name) {
        auto item = mem.find({name,0});

        if (item != std::end(mem)) return *item._M_cur->_M_v();
        else {
            mem.insert({name, 0});
            return 0;
        }
    }
};






















    

