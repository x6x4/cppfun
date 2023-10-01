#pragma once
#include <algorithm>
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <sys/types.h>
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

using addr_t = u_int8_t;

class Operand {

    int value;    
    std::string label;
    addr_t addr;
    

public:

    void set_addr (addr_t a) { addr = a; }
    std::string &get_name () { return label; }
    int get_val () { return value; }

    Operand& operator= (std::string name) {
        label = name;
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
    addr_t addr;
    Operator oper;
    Operand op1;
    Operand op2;

public:

    //Operator *get_op() const { return op; }
};

std::ostream &operator<<(std::ostream &os, Command &cmd);










