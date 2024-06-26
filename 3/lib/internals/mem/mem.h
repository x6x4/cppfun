/** @file mem/mem.h
 *  
 *  Text and data areas layout.
 */
#pragma once
#include "fwd_mem.h"

/**
 * @class ProgramMemory
 * @brief Text section with infinite capacity. 
 *
 * Stores special-purpose program counter register and some flag registers.
 */
class ProgramMemory {

friend PCRegister;
friend CPU;

    Text text;
    my_std::Vec<std::size_t> sp_regs = my_std::Vec<std::size_t>(2);
    std::size_t pc_num = 0, zf_num = 1;

    void load (const SafeText &mtext);
    void set_spreg (std::size_t num, std::size_t val);
    const Command &fetch ();
    int operator[] (std::size_t num) const { return sp_regs.at(num); }
    std::size_t get_pc () { return sp_regs[pc_num]; }
    bool is_over ();
    void clear();

public:

    ProgramMemory () = default;
    ~ProgramMemory(); 
    ProgramMemory (const ProgramMemory&) = default;
    ProgramMemory (ProgramMemory&&) = default;
    ProgramMemory &operator= (ProgramMemory &&_pm) = default;

    /**
    * @brief Prints contents of special-purpose register block.
    * @param os The output stream for printing.
    */
    void print_regblock(std::ostream &os) const;

    /**
    * @brief Clears contents of special-purpose register block.
    */  
    void clear_regs () { for (auto &reg : sp_regs) reg = 0; }

    /**
    * @brief Overloaded friend function to stream the ProgramMemory object.
    *
    * @param os The output stream where the ProgramMemory object will be streamed.
    * @param pm The ProgramMemory object to be streamed.
    * @return A reference to the output stream after streaming the ProgramMemory object.
    */
    friend std::ostream &operator<<(std::ostream &os, ProgramMemory &pm);
    
};


/**
 * @class DataMemory
 * @brief Data section with finite capacity. 
 *
 * Stores integer data sequantially.  
 */
class DataMemory {

friend DataCell;
friend CPU;

    std::size_t cap = 0;
    Data data = Data(cap);
    void load (const Data &mdata);
    int operator[] (std::size_t num) const { return data.at(num); }

public:

    /**
    * @brief Constructor for the DataMemory class.
    *
    * @param _cap The number of data cells.
    */
    DataMemory (std::size_t _cap) : cap (_cap) {};

    /**
    * @brief Trivial accessor for capacity.
    *
    * @return The number of data cells.
    */
    std::size_t getCap() const { return cap; }

    /**
    * @brief Overloaded friend function to stream the DataMemory object.
    *
    * @param os The output stream where the DataMemory object will be streamed.
    * @param dm The DataMemory object to be streamed.
    * @return A reference to the output stream after streaming the DataMemory object.
    */
    friend std::ostream &operator<<(std::ostream &os, DataMemory &dm);

};


/**
 * @class Memory
 * @brief Private element of CPU, aggregates memory of data and programs.
 */
class Memory {
friend CPU;
friend PCRegister;
friend DataCell;

    ProgramMemory pm;
    DataMemory dm;

    Memory (std::size_t sz) : dm(sz) { };
};
