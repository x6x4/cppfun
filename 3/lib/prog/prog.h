#pragma once
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <sstream>


void check_id (std::string name);

class ID {

    std::string id;
    
public:

    std::string &get_id () { return id; }

    void operator=(std::string name) {
        check_id (name);
        id = name;
    } 

    friend std::istream& operator>> (std::istream& is, ID &id) {
        is >> id.id;
        return is;
    }
};


class Operand {

    int value;    
    std::string cmd;

public:

    std::string &get_name () { return cmd; }
    int get_val () { return value; }

    Operand& operator= (std::string name) {
        cmd = name;
        return *this;
    }

    friend std::istream& operator>> (std::istream& is, Operand &op) {
        is >> op.value;
        return is;
    }

};


class Operator {
    std::string cmd;

public:

    std::string &get_name () { return cmd; }

    Operator& operator= (std::string name) {
        cmd = name;
        return *this;
    }

public:

    void operator() () const;
};



class Command {

public:
    ID label;
    Operator oper;
    Operand op1;
    Operand op2;

public:

    //Operator *get_op() const { return op; }
    

};


Command make_cmd (std::string command);
Command make_data (std::string command);

std::ostream &operator<<(std::ostream &os, Command &cmd);



//  PROGRAM MEMORY

class Command;
using prog_t = std::vector<Command>;
class ProgramMemory;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  Code section with infinite capacity. 
 *  Stores special-purpose program counter register.  
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class ProgramMemory {

    
    //reg &PC;

public:

    prog_t prog = {};
    
    const Command &get_cur_cmd () const;

    
};

std::ostream &operator<<(std::ostream &os, ProgramMemory &pm);

void load_program (const char *s, ProgramMemory &pm);



