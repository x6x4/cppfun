/** @file prog/prog.h
 *  This file contains basic classes of QASM internal structure,   
 *  such as commands and identifiers.
 */

#pragma once
#include <cstddef>
#include <fstream>


//  Q8 has bitness 8
using addr_t = u_int8_t;


//  ID

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


//  BASE CLASS - OPERATOR

class Operator {
protected:
    std::string mnem;

public:

    const std::string &mnemonics () const { return mnem; }

    Operator& operator= (std::string _mnem) {
        mnem = _mnem;
        return *this;
    }

public:

    void operator() () const;
};

std::ostream &operator<<(std::ostream &os, const Operator &op);

//  BASE CLASS - OPERAND

class Operand {

protected:

    int value = 0;

    virtual void print (std::ostream &os) const = 0;
    
public:

    Operand () {};
    Operand(int val) : value(val) {};
    int val() const { return value; }
    void set(int _val) { value = _val; }

    friend std::ostream &operator<<(std::ostream &os, const Operand &opd);
}; 


//  DERIVED FROM OPERAND

class Register : public Operand {
    std::size_t num = 0;

protected:
    void print (std::ostream &os) const override;

public:
    Register(std::size_t number) : num(number) {};
};

class DataLabel : public Operand {

protected:
    void print (std::ostream &os) const override;

    addr_t label = 0;

public:
    DataLabel(addr_t _lbl) : label(_lbl) {};
    //DataLabel &Datalabel(DataLabel dtlbl) { *this = dtlbl; return *this; } 
    //DataLabel &Datalabel(DataLabel &&dtlbl) { *this = dtlbl; return *this; }
};



//  BASE CLASS - COMMAND

class Command {

protected:
    ID lbl;
    Operator oper;

    virtual void print (std::ostream &os) const = 0;

public:

    const ID &label() const      { return lbl;  }
    const Operator &op() const   { return oper; }

    void exec ();
    friend std::ostream &operator<<(std::ostream &os, Command &cmd);
};



//  DERIVED FROM COMMAND

class UnaryCommand : public Command {

    Operand &opd1;

protected:
    void print (std::ostream &os) const override;

public:

    /**
    * @brief       Initing unary command constructor (all fields)
    * @param       _lbl   command label
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @return      Created command
    */
    UnaryCommand(std::string _lbl, Operator _oper, Operand &_opd1) : opd1(_opd1){
        Command::lbl = _lbl;
        Command::oper = _oper;
    }

    /**
    * @brief       Initing unary command constructor
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @return      Created command
    */
    UnaryCommand(Operator _oper, Operand &_opd1) : opd1(_opd1) {
        Command::oper = _oper;
    }

};




class BinaryCommand : public Command {
    
    Operand &opd1;
    Operand &opd2;

protected:
    void print (std::ostream &os) const override;

public:

    /**
    * @brief       Initing binary command constructor (all fields)
    * @param       _lbl   command label
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @param       _opd2  2nd operand
    * @return      Created command
    */
    BinaryCommand(std::string _lbl, Operator _oper, Operand &_opd1, Operand &_opd2)
        : opd1(_opd1), opd2(_opd2) {
        Command::lbl = _lbl;
        Command::oper = _oper;
    }

    /**
    * @brief       Initing binary command constructor
    * @param       _oper  operator
    * @param       _opd1  1st operand
    * @param       _opd2  2nd operand
    * @return      Created command
    */
    BinaryCommand(Operator _oper, Operand &_opd1, Operand &_opd2) 
        : opd1(_opd1), opd2(_opd2) {
        Command::oper = _oper;
    }

};












