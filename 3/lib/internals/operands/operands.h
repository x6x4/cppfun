/** @file operands/operands.h
 *  
 *  Arch-dependent operands.
 */
#pragma once

#include "../cpu/cpu.h"


/**
 * @class GPRegister
 * @brief Small and fast memory cell inside CPU. 
 */
class GPRegister : public Operand {
friend SPRegister;
friend DataCell;

    std::size_t num = 0;

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    GPRegister() {};
    ~GPRegister () override = default;

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
    GPRegister(std::size_t number) : num(number) {};
};

/**
 * @class SPRegister
 * @brief Small and fast memory cell inside Program Memory. 
 */
class SPRegister : public GPRegister {

    void print (std::ostream &os) const override;
    void load_to (CPU &cpu) const override;
    void load_from (CPU &cpu) override;

public:
    SPRegister () {};
    ~SPRegister () override = default;

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
    SPRegister (std::size_t _val) { value = _val; }

    /** 
    * @brief Trivial setter for special-purpose register number.
    */
    void set_num (std::size_t _num) { num = _num; }  
};

/**
 * @class DataCell
 * @brief Big and slow memory cell inside Data Memory. 
 */
class DataCell : public GPRegister {

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
    * @param _val Data cell value
    */
    DataCell (std::size_t _val) { value = _val; }

    /** 
    * @brief Trivial setter for data cell number.
    */
    void set_num (std::size_t _num) { num = _num; }  
};
