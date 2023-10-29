/** @file operands/operands.h
 *  
 *  Arch-dependent operands.
 */
#pragma once

#include "../cpu/cpu.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Small and fast memory cell inside CPU. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class GPRegister : public Operand {

protected:
    std::size_t num = 0;

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    GPRegister() {};
    ~GPRegister () override = default;
    std::unique_ptr<Operand> clone () const override;

    GPRegister(std::size_t number) : num(number) {};
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Small and fast memory cell inside Program Memory. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class SPRegister : public GPRegister {

protected:

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    SPRegister () {};
    ~SPRegister () override = default;
    std::unique_ptr<Operand> clone () const override;

    SPRegister (std::size_t _val) { value = _val; }
    void set_num (std::size_t _num) { num = _num; }  
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Big and slow memory cell inside Data Memory. 
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
class DataCell : public GPRegister {
protected:

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    DataCell () {};
    ~DataCell () override = default;
    std::unique_ptr<Operand> clone () const override;

    DataCell (std::size_t _val) { value = _val; }
    void set_num (std::size_t _num) { num = _num; }  
};
