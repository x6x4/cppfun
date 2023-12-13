/** @file operands/operands.h
 *  
 *  Arch-dependent operands.
 */
#pragma once

#include "../cpu/cpu.h"
#include <cstddef>

using ImmOperand = Operand;
using GPRegister = NumberedCell;

/**
 * @class String (aka GPRegister)
 * @brief Provides interface for strings operations, 
    such as storing.
 */
class String : public Operand {

    const Data *store = nullptr;
    
    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

    public:
    String() {};
    ~String () override = default;

    //virtual void printStr() {};

    /** 
    * @brief Trivial accessor for store.
    * @return const ref to store
    */
    const auto &getStore () { return *store; }

    /**
    * @brief Creates a copy of the Operand object
    *
    * @note This is an overriden function. 
    * @see Operand
    * 
    * @return Pointer to the cloned Operand object
    */
    std::unique_ptr<Operand> clone () const override;

    /**
    * @brief Constructor for the String class.
    *
    * @param 
    */
    String (int val) : Operand(val) {};
};

/**
 * @class NumberedCell (aka GPRegister)
 * @brief Small and fast memory cell inside CPU. 
 */
class NumberedCell : public Operand {
friend PCRegister;
friend DataCell;

    std::size_t num = 0;

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    NumberedCell() {};
    ~NumberedCell () override = default;

    /**
    * @brief Creates a copy of the Operand object
    *
    * @note This is an overriden function. 
    * @see Operand
    * 
    * @return Pointer to the cloned Operand object
    */
    std::unique_ptr<Operand> clone () const override;

    /**
    * @brief Constructor for the GPRegister class.
    *
    * @param number Register number
    */
    NumberedCell(std::size_t number) : num(number) {};
};

/**
 * @class PCRegister
 * @brief Instruction pointer register. 
 */
class PCRegister : public NumberedCell {

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    PCRegister () {};
    ~PCRegister () override = default;

    /**
    * @brief Creates a copy of the Operand object
    *
    * @note This is an overriden function. 
    * @see Operand
    * 
    * @return Pointer to the cloned Operand object
    */
    std::unique_ptr<Operand> clone () const override;

    /**
    * @brief Constructor for the SPRegister class.
    *
    * @param _val Register value
    */
    PCRegister (std::size_t _val) { value = _val; }

    /** 
    * @brief Trivial setter for special-purpose register number.
    */
    void set_num (std::size_t _num) { num = _num; }  
};

/**
 * @class DataCell
 * @brief Big and slow memory cell inside Data Memory. 
 */
class DataCell : public NumberedCell {

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    DataCell () {};
    ~DataCell () override = default;

    /**
    * @brief Creates a copy of the Operand object
    *
    * @note This is an overriden function. 
    * @see Operand
    * 
    * @return Pointer to the cloned Operand object
    */
    std::unique_ptr<Operand> clone () const override;

    /**
    * @brief Constructor for the DataCell class.
    *
    * @param _num Data cell _num
    */
    DataCell (std::size_t _num) { num = _num; }
};
