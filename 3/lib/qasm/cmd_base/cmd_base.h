/** @file qasm/cmd_base.h
 *  This file contains basic classes of QASM command structure.
 */

#pragma once
#include <cstddef>
#include <cstring>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <unordered_set>
#include <vector>

///  fwd
using addr_t = std::size_t;
class UnaryOperator;
class BinaryOperator;
using unary_instr_set = std::unordered_set<UnaryOperator>;
using binary_instr_set = std::unordered_set<BinaryOperator>;
class CPU;
using Mnemonic = std::string;
class ExecUnit;


//  ID  //

/**
* @brief       Check if string can be ID
* @param       str - string to check
* @exception   std::logic_error  cause why string can't be ID
*/
void check_id (std::string str);

class ID {

    std::string id;
    addr_t addr;
    
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
    void set_addr (addr_t _addr) { addr = _addr; }

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


class Operand {

protected:

    int value = 0;

    virtual void print (std::ostream &os) const {};
    virtual void load (CPU &cpu) const {};
    
public:

    virtual ~Operand () = default;

    Operand () {};
    Operand(int val) : value(val) {};
    int val() const { return value; }
    void set(int _val) { value = _val; }

    friend std::ostream &operator<<(std::ostream &os, const Operand &opd);
};

class Register : public Operand {

protected:
    std::size_t num = 0;

    void print (std::ostream &os) const override;
    void load (CPU &cpu) const override;

public:
    ~Register () override = default;

    Register(std::size_t number) : num(number) {};
};


//  BASE CLASS - OPERATOR  //


class Operator {

protected:
    Mnemonic mnem;

public:
    Operator () {};
    
    Operator(Mnemonic _mnem) { mnem = _mnem; }

    const std::string &mnemonics () const { return mnem; }
};

std::ostream &operator<<(std::ostream &os, const Operator &op);

class UnaryOperator : public Operator {

protected:
    void (*oper)(Operand &opd1);

public:
    UnaryOperator() {};
    UnaryOperator(const UnaryOperator &copy) = default;
    UnaryOperator(UnaryOperator &copy) = default;
    UnaryOperator &operator= (UnaryOperator &copy) = default;
    UnaryOperator(UnaryOperator &&move) = default;
    UnaryOperator &operator= (UnaryOperator &&move) = default;

    UnaryOperator(Mnemonic _mnem) : Operator(_mnem ) {}

    void operator() (Operand &opd1) { oper(opd1); }
    bool operator== (const UnaryOperator&) const;
};

class BinaryOperator : public Operator {

protected:
    void (*oper)(Operand &opd1, Operand &opd2);

public:

    BinaryOperator () {};
    BinaryOperator(BinaryOperator &copy) = default;
    BinaryOperator(const BinaryOperator &copy) = default;
    BinaryOperator &operator= (BinaryOperator &copy) = default;
    BinaryOperator(BinaryOperator &&move) = default;
    BinaryOperator &operator= (BinaryOperator &&move) = default;

    BinaryOperator(Mnemonic _mnem) : Operator(_mnem ) {}
    
    void operator() (Operand &opd1, Operand &opd2) { oper(opd1, opd2); }
    bool operator== (const BinaryOperator&) const;
};   


//  INSTRUCTION SET  //

namespace std {

    template<> struct hash<UnaryOperator>
    {
        std::size_t operator()(const UnaryOperator& oper) const noexcept
        {
            std::size_t hash = std::hash<Mnemonic>()(oper.mnemonics());
            return hash;
        }
    };

    template<> struct hash<BinaryOperator>
    {
        std::size_t operator()(const BinaryOperator& oper) const noexcept
        {
            std::size_t hash = std::hash<Mnemonic>()(oper.mnemonics());
            return hash;
        }
    };
}

class InstrSet {
    unary_instr_set uset;
    binary_instr_set bset;

public:
    InstrSet (unary_instr_set& _uset, binary_instr_set& _bset); 

    UnaryOperator  &FindUnOper (Mnemonic str);
    BinaryOperator &FindBinOper (Mnemonic str);
};


//  BASE CLASS - COMMAND  //

class Command {

friend ExecUnit;

protected:
    ID lbl = "";
    
public:

    virtual void print (std::ostream &os) const = 0;

public:

    const ID &label() const      { return lbl;  }

    virtual void load (CPU &cpu) = 0;
    virtual void exec (CPU &cpu) const = 0;
    
    friend std::ostream &operator<<(std::ostream &os, Command &cmd);
};


//  DERIVED FROM COMMAND

class UnaryCommand : public Command {

    std::shared_ptr<Operand> opd1;
    UnaryOperator unoper;

protected:

    void print (std::ostream &os) const override;

    void load (CPU &cpu) override;
    void exec (CPU &cpu) const override;

public:

    /**
    * @brief       Initing unary command constructor (all fields)
    * @param       _lbl   command label
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @return      Created command
    */
    UnaryCommand(ID _lbl, UnaryOperator _oper, std::shared_ptr<Operand> _opd1) : opd1(_opd1), unoper(_oper) {
        Command::lbl = _lbl;
    }

    /**
    * @brief       Initing unary command constructor
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @return      Created command
    */
    UnaryCommand(UnaryOperator _oper, std::shared_ptr<Operand> _opd1) : opd1(_opd1), unoper(_oper) {}

};


class BinaryCommand : public Command {
    
    std::shared_ptr<Operand> opd1;
    std::shared_ptr<Operand> opd2;
    BinaryOperator binoper;

protected:

    void print (std::ostream &os) const override;

    void load (CPU &cpu) override;
    void exec (CPU &cpu) const override;

public:

    /**
    * @brief       Initing binary command constructor (all fields)
    * @param       _lbl   command label
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @param       _opd2  2nd operand
    * @return      Created command
    */
    BinaryCommand(ID _lbl, BinaryOperator _oper, std::shared_ptr<Operand> _opd1, std::shared_ptr<Operand> _opd2)
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
    BinaryCommand(BinaryOperator _oper, std::shared_ptr<Operand>_opd1, std::shared_ptr<Operand> _opd2) 
        : opd1(_opd1), opd2(_opd2), binoper(_oper)  {}

};


