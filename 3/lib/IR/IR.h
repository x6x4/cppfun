/** @file IR/IR.h
 *  
 *  Arch-independent basic classes of IR.
 */

#pragma once
#include "fwd_IR.h"


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
    
    addr_t get_addr () const { return addr; }
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

    bool operator== (const ID& id) const { return id.get_id() == get_id();}
};

std::ostream &operator<<(std::ostream &os, const ID &id);


//  BASE CLASS - OPERAND  //

class Operand {

friend UnaryCommand;
friend BinaryCommand;
friend CPU;
friend Cache;

protected:

    int value = 0;

    virtual void print (std::ostream &os) const {};
    virtual void load_to (CPU &cpu) const = 0;
    virtual void load_from (CPU &cpu) = 0;
    
public:

    Operand () {};
    virtual ~Operand () = default;
    virtual std::unique_ptr<Operand> clone () const = 0;

    Operand(int val) : value(val) {};
    int val() const { return value; }
    void set(int _val) { value = _val; }

    friend std::ostream &operator<<(std::ostream &os, const Operand &opd);
};


//  CACHE  //

class Cache {
friend UnaryCommand;
friend BinaryCommand;
friend CPU;

    std::unique_ptr<Operand> opd1 = nullptr;
    std::unique_ptr<Operand> opd2 = nullptr;

public:

    void load_opd1 (Operand &_opd1, CPU &cpu);
    void load_opd2 (Operand &_opd2, CPU &cpu);

    void clear ();
};


//  BASE CLASS - OPERATOR  //

class Operator {

protected:
    Mnemonic mnem;
    short time;

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
    UnaryOperator &operator= (const UnaryOperator &copy) = default;
    UnaryOperator(UnaryOperator &&move) = default;
    UnaryOperator &operator= (UnaryOperator &&move) = default;

    UnaryOperator(Mnemonic _mnem) : Operator(_mnem ) {}

    void operator() (Operand &opd1) const { oper(opd1); }
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
    
    void operator() (Operand &opd1, Operand &opd2) const { oper(opd1, opd2); }
    bool operator== (const BinaryOperator&) const;
};   


//  INSTRUCTION SET  //

namespace std {

    template<> struct hash<ID>
    {
        std::size_t operator()(const ID& id) const noexcept
        {
            std::size_t hash = std::hash<std::string>()(id.get_id());
            return hash;
        }
    };   

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

    virtual void load (Cache &cache, CPU &cpu) const = 0;
    virtual void exec (Cache &cache, CPU &cpu) const = 0;
    
public:
    virtual ~Command() = default;
    virtual Command* clone () const = 0;
    virtual void print (std::ostream &os) const = 0;

public:

    const ID &label() const      { return lbl;  }
    
    friend std::ostream &operator<<(std::ostream &os, Command &cmd);
};


//  DERIVED FROM COMMAND

class UnaryCommand : public Command {

    std::unique_ptr<Operand> opd1;
    UnaryOperator unoper;

protected:
    void load (Cache &cache, CPU &cpu) const override;
    void exec (Cache &cache, CPU &cpu) const override;
    void print (std::ostream &os) const override;

public:

    ~UnaryCommand() override {}

    UnaryCommand (UnaryCommand &&) = default;

    UnaryCommand* clone () const override;

    /**
    * @brief       Initing unary command constructor (all fields)
    * @param       _lbl   command label
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @return      Created command
    */
    UnaryCommand(ID _lbl, UnaryOperator _oper, std::unique_ptr<Operand> _opd1);

    /**
    * @brief       Initing unary command constructor
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @return      Created command
    */
    UnaryCommand(UnaryOperator _oper, std::unique_ptr<Operand> _opd1);

};

class BinaryCommand : public Command {
    
    std::unique_ptr<Operand> opd1;
    std::unique_ptr<Operand> opd2;
    BinaryOperator binoper;

protected:
    void load (Cache &cache, CPU &cpu) const override;
    void exec (Cache &cache, CPU &cpu) const override;
    void print (std::ostream &os) const override;

public:

    ~BinaryCommand() override {}

    BinaryCommand (BinaryCommand &&) = default;

    BinaryCommand* clone () const override;

    /**
    * @brief       Initing binary command constructor (all fields)
    * @param       _lbl   command label
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @param       _opd2  2nd operand
    * @return      Created command
    */
    BinaryCommand(ID _lbl, BinaryOperator _oper, std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2);

    /**
    * @brief       Initing binary command constructor
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @param       _opd2  2nd operand
    * @return      Created command
    */
    BinaryCommand(BinaryOperator _oper, std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2);

};

