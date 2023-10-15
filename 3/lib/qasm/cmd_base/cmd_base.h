/** @file qasm/cmd_base.h
 *  This file contains basic classes of QASM command structure.
 */

#pragma once
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <unordered_set>
#include <vector>


//  ID  //

/**
* @brief       Check if string can be ID
* @param       str - string to check
* @exception   std::logic_error  cause why string can't be ID
*/
void check_id (std::string str);

class ID {

    std::string id;
    
public:

    ID() {};

    ID(const std::string &str) {
        check_id (str);
        id = str;
    }
    ID(const char *str) {
        check_id (str);
        id = str;
    }

    const std::string &get_id () const { return id; }

    ID &operator=(std::string str) {
        ID buf(str);
        swap(buf);
        return *this;
    } 

    void swap (ID other) {
        std::swap(other.id, id);
    }

    friend std::istream& operator>> (std::istream& is, ID &id) {
        is >> id.id;
        return is;
    }
};

std::ostream &operator<<(std::ostream &os, const ID &id);


//  BASE CLASS - OPERAND  //

class CPU;

class Operand {

protected:

    int value = 0;

    virtual void print (std::ostream &os) const = 0;
    
public:

    Operand () {};
    Operand(int val) : value(val) {};
    int val() const { return value; }
    void set(int _val) { value = _val; }
    virtual void load (CPU& cpu) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Operand &opd);
};

class Register : public Operand {
    
    std::size_t num = 0;

protected:
    void print (std::ostream &os) const override;

public:
    Register(std::size_t number) : num(number) {};
    void load(CPU &cpu) const override;
};


//  BASE CLASS - OPERATOR  //

using Mnemonic = std::string;

class Operator {

protected:
    Mnemonic mnem;

public:
    Operator(Mnemonic _mnem) { mnem = _mnem; }
    const std::string &mnemonics () const { return mnem; }
};

std::ostream &operator<<(std::ostream &os, const Operator &op);

class UnaryOperator : public Operator {

protected:
    void (*oper)(Operand &opd1);

public:

    void operator() (Operand &opd1) { oper(opd1); }
    UnaryOperator(Mnemonic _mnem) : Operator(_mnem ) {}
};

class BinaryOperator : public Operator {

protected:
    void (*oper)(Operand &opd1, Operand &opd2);

public:
    void operator() (Operand &opd1, Operand &opd2) { oper(opd1, opd2); }
    BinaryOperator(Mnemonic _mnem) : Operator(_mnem ) {}
};   

//  INSTRUCTION SET  //

namespace std {

    template<> struct hash<UnaryOperator>
    {
        std::size_t operator()(const UnaryOperator &oper) const noexcept
        {
            std::size_t hash = std::hash<Mnemonic>()(oper.mnemonics());
            return hash;
        }
    };

    template<> struct hash<BinaryOperator>
    {
        std::size_t operator()(const BinaryOperator &oper) const noexcept
        {
            std::size_t hash = std::hash<Mnemonic>()(oper.mnemonics());
            return hash;
        }
    };
}

using unary_instr_set = std::unordered_set<UnaryOperator>;
using binary_instr_set = std::unordered_set<BinaryOperator>;


class InstrSet {
    unary_instr_set uset;
    binary_instr_set bset;

public:
    InstrSet (unary_instr_set& _uset, binary_instr_set& _bset); 

    UnaryOperator  &FindUnOper (Mnemonic str);
    BinaryOperator &FindBinOper (Mnemonic str);
};

bool operator== (const UnaryOperator &op1, const UnaryOperator &op2);
bool operator== (const BinaryOperator &op1, const BinaryOperator &op2);


//  BASE CLASS - COMMAND  //

class Command {

protected:
    ID lbl = "";

public:
    virtual void print (std::ostream &os) const = 0;

public:

    const ID &label() const      { return lbl;  }

    virtual void load (CPU &cpu) = 0;
    virtual void exec (CPU &cpu) = 0;
    
    friend std::ostream &operator<<(std::ostream &os, Command &cmd);
};

using mprog = std::vector<Command*>;
void print_mprog (std::ostream &os, const mprog& prog);


//  DERIVED FROM COMMAND

class UnaryCommand : public Command {

    Operand &opd1;
    UnaryOperator unoper;

protected:
    void print (std::ostream &os) const override;

    void load (CPU &cpu) override;
    void exec (CPU &cpu) override;

public:

    /**
    * @brief       Initing unary command constructor (all fields)
    * @param       _lbl   command label
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @return      Created command
    */
    UnaryCommand(std::string _lbl, UnaryOperator _oper, Operand &_opd1) : opd1(_opd1), unoper(_oper) {
        Command::lbl = _lbl;
    }

    /**
    * @brief       Initing unary command constructor
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @return      Created command
    */
    UnaryCommand(UnaryOperator _oper, Operand &_opd1) : opd1(_opd1), unoper(_oper) {}

};


class BinaryCommand : public Command {
    
    Operand &opd1;
    Operand &opd2;
    BinaryOperator binoper;

protected:
    void print (std::ostream &os) const override;

    void load (CPU &cpu) override;
    void exec (CPU &cpu) override;

public:

    /**
    * @brief       Initing binary command constructor (all fields)
    * @param       _lbl   command label
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @param       _opd2  2nd operand
    * @return      Created command
    */
    BinaryCommand(std::string _lbl, BinaryOperator _oper, Operand &_opd1, Operand &_opd2)
        : opd1(_opd1), opd2(_opd2), binoper(_oper)  {
        Command::lbl = _lbl;
    }

    /**
    * @brief       Initing binary command constructor
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @param       _opd2  2nd operand
    * @return      Created command
    */
    BinaryCommand(BinaryOperator _oper, Operand &_opd1, Operand &_opd2) 
        : opd1(_opd1), opd2(_opd2), binoper(_oper)  {}

};


