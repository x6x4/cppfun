/** @file IR/IR.h
 *  
 *  Arch-independent basic classes of IR.
 */

#pragma once
#include "fwd_IR.h"
#include <initializer_list>


/* 
 *  The label class.                                             
 *                                                               
 *  ID string must consist of up to 8 latin letters.             
 *                                                               
 */

struct ID {

    std::string id;
    addr_t addr;
    bool is_ascii = 0;
    size_t end_of_mem = 0;

    void swap (ID other) { std::swap(other.id, id); }

    ID() {};

    /**
    * @brief Constructor for the ID class.
    *
    * @param str The string for the ID.
    */
    ID(const char *str);

    /**
    * @brief Copy constructor for the ID class.
    *
    * @param other The other ID object.
    */
    constexpr ID(const ID& other) = default;

    /**
    * @brief Assignment operator for ID.
    *
    * @param other The ID object to assign.
    */    
    ID &operator=(ID other);

    /**
    * @brief Overloaded friend function to stream the ID object.
    *
    * @param is The input stream the ID object will be streamed from.
    * @param id The ID object to be streamed from.
    * @return A reference to the input stream after streaming the ID object.
    */
    friend std::istream& operator>> (std::istream& is, ID &id);

    /**
    * @brief Equality comparison operator for ID.
    *
    * @param other The ID object to compare with.
    * @return True if the IDs are the same, false otherwise.
    */    
    bool operator== (const ID& _id) const { return _id.id == id;}
};

/**
* @brief       Check if string can be ID
* @param       str - string to check
* @throw   std::logic_error  cause why string can't be ID
*/
void check_id (const std::string &str);

/**
* @brief Overloaded output operator to stream the ID object.
*
* @param os The output stream where the ID object will be streamed.
* @param id The ID object to be streamed.
* @return A reference to the output stream after streaming the ID object.
*/
std::ostream &operator<<(std::ostream &os, const ID &id);


/*
 *  The base abstract class of all operands.                     
 *                                                               
 *  Contains value, is an immediate operand at its essence.      
 *  Also contains some general interface.                        
 *  Can be loaded from CPU and back.                              
 *                                                               
 *  Compiler recognize GPRegisters, SPRegisters, DataCells,       
 *  ImmOperands and Strings.                                             
 *                                                               
 */

class Operand {

friend Command;
friend CPU;
friend CPU_Cache;

protected:

    int value = 0;

    /**
    * @brief Prints operand contents.
    * @param os The output stream for printing.
    *
    * @note This is a virtual function.     
    * @see GPRegister, SPRegister, DataCell
    */
    virtual void print (std::ostream &os) const { os << value; }

    /**
    * @brief Loads operand from cache to CPU.
    * 
    * @param cpu The CPU object to which operand are loaded.
    *
    * @note This is a virtual function.     
    * @see GPRegister, SPRegister, DataCell
    */
    virtual void load_to (CPU &cpu) const {};

    /**
    * @brief Loads operand from CPU to cache.
    * 
    * @param cpu The CPU object from which operand are loaded.
    *
    * @note This is a virtual function.     
    * @see GPRegister, SPRegister, DataCell
    */ 
    virtual void load_from (CPU &cpu) {};

    /**
    * @brief Creates a copy of the Operand object
    *
    * @note This is a virtual function. 
    * @see GPRegister, SPRegister, DataCell
    * 
    * @return Pointer to the cloned Operand object
    */
    virtual std::unique_ptr<Operand> clone () const { return std::make_unique<Operand>(*this);  }

    
public:

    Operand () {};
    virtual ~Operand () = default;

    /** 
    * @brief Trivial setter for Operand value.
    */
    void set(int _val) noexcept{ value = _val; }

    /**
    * @brief Constructor for the Operand class.
    *
    * @param val The value for the Operand.
    */
    Operand(int val) : value(val) {};

    /**
    * @brief Trivial accessor for operand value.
    *
    * @return The value of the operand.
    */
    int val() const noexcept { return value; }

    /**
    * @brief Overloaded friend function to stream the Operand object.
    *
    * @param os The output stream where the Operand object will be streamed.
    * @param opd The Operand object to be streamed.
    * @return A reference to the output stream after streaming the Operand object.
    */
    friend std::ostream &operator<<(std::ostream &os, const Operand &opd);
};


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Internal private CPU structure.                              *
 *  It loads copy of operands from CPU.                          *
 *  Execution unit executes command on them then,                *
 *  and load them back to CPU.                                   *
 *                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class CPU_Cache {
friend Command;
friend CPU;

    my_std::Vec<std::unique_ptr<Operand>> opds;
    CPU *cpu = nullptr;
    CPU_Cache (CPU* _parent) : cpu(_parent) {};
    CPU_Cache (const CPU_Cache &c);

    void load_opds (const my_std::Vec<std::unique_ptr<Operand>> &_opds);

    void clear ();
};

/*
 *  The base abstract class of all operators.                    
 *                                                               
 *  Operator consists of operation, time of its execution        
 *  and their assembler mnemonics (parsed by compiler).          
 *  Compiler recognize only and binary operators.          
 *  Any instruction sets are formed from them.                   
 *                                                               
 *  Result of an operation is always put to first operand.                                                                       
 */

class OperatorBase {

protected:
    Mnemonic mnem;
    short time;

public:
    OperatorBase () {};

    /**
    * @brief Constructor for the Operator class.
    *
    * @param _mnem The mnemonic string for the Operator.
    */
    OperatorBase(const Mnemonic _mnem) { mnem = _mnem; }

    /**
    * @brief Trivial accessor for operator mnemonics.
    *
    * @return The mnemonic string of the operator.
    */
    const std::string &mnemonics () const noexcept { return mnem; }
};

/**
* @brief Overloaded output operator to stream the Operator object.
*
* @param os The output stream where the Operator object will be streamed.
* @param op The Operator object to be streamed.
* @return A reference to the output stream after streaming the Operator object.
*/
std::ostream &operator<<(std::ostream &os, const OperatorBase &op);


/*
 *  Derived from Operator.                                       
 *                                                               
 *  Consists of operation acting on operands.      
 *                                                               
 */
//template <class ... Args>
class Operator : public OperatorBase {

protected:
    void (*oper)(my_std::Vec<std::unique_ptr<Operand>> &opds);

public:
    Operator() {};

    Operator(const Operator &copy) = default;
    Operator(Operator &copy) = default;
    Operator &operator= (const Operator &copy) = default;

    Operator(Operator &&move) = default;
    Operator &operator= (Operator &&move) = default;

    /**
    * @brief Constructor for the Operator class.
    *
    * @param _mnem The mnemonic string for the Operator.
    */
    Operator(const Mnemonic &_mnem) : OperatorBase(_mnem) {}

    /**
    * @brief Function call operator for Operator.
    *
    * @param opd1 The first operand reference.
    */
    void operator() (my_std::Vec<std::unique_ptr<Operand>> &_opds) const { oper(_opds); }

    /**
    * @brief Equality comparison operator for Operator.
    *
    * @param other The Operator object to compare with.
    * @return True if the mnemonics are the same, false otherwise.
    */    
    bool operator== (const Operator& other) const noexcept;
};


namespace std {

    template<> struct hash<ID>
    {
        std::size_t operator()(const ID& _id) const noexcept
        {
            std::size_t hash = std::hash<std::string>()(_id.id);
            return hash;
        }
    };   

    template<> struct hash<Operator>
    {
        std::size_t operator()(const Operator& oper) const noexcept
        {
            std::size_t hash = std::hash<Mnemonic>()(oper.mnemonics());
            return hash;
        }
    };
}

/*
 * Contains a table of operators and its mnemonics.              
 *                                                               
 * By design, user instruction set links as a separate library.  
 * User must implement own table for custom operators 
 * and construct own instruction set from them.                                                             
 */

using instr_set = std::unordered_set<Operator>;


class InstrSet {
    const instr_set iset;

public:

    /**
    * @brief Constructor for the InstrSet class.
    *
    * @param _iset A const reference to instr_set object.
    */
    InstrSet(const instr_set& _iset) : iset(_iset) {}; 

    /**
    * @brief Finds an operator by its mnemonics.
    *
    * @param str A const reference to mnemonic string of the operator.
    * @throw std::logic_error "Operator not found"
    * @return A reference to the Operator object.
    */
    Operator &FindOper(const Mnemonic &str) const;
};


/* 
 *  The base abstract class of all commands.                     
 *                                                               
 *  Contains label and some general interface.                   
 *  Consists of operands and operation on them.                  
 *  Is always executed in cache.                                 
 *  Result of an operation is always put to first operand.        
 *                                                               
 *  Compiler recognize only 1 to 3 arguments in command.           
 *                                                               
 */

class CommandBase {

protected:
    ID lbl = "";

    /**
    * @brief Executes the command operation in the cache.
    * 
    * @param cache The cache object where the command operation is executed.
    *
    * @note This is a pure virtual function.
    * @see Command, Command
    */
    virtual void exec_in_cache (CPU_Cache &cache) const = 0;

    /**
    * @brief Loads command operands from CPU to cache.
    * 
    * @param cache The cache object to which command's operands are loaded.
    *
    * @note This is a pure virtual function.
    * @see Command, Command
    */
    virtual void load_to_cache (CPU_Cache &cache) const = 0;

    /**
    * @note This is a pure virtual function.
    * @see Command, Command
    */
    virtual void print (std::ostream &os) const = 0;

public:

    /**
    * @note This is a pure virtual function.
    * @see Command, Command
    */
    virtual ~CommandBase() = default;

    /**
    * @note This is a pure virtual function.
    * @see Command, Command
    */
    virtual CommandBase* clone () const = 0;

    /**
    * @brief Returns the label of the command.
    *
    * @return A constant reference to the label.
    */
    const ID &label() const { return lbl;  }
    
    /**
    * @brief Overloaded friend function to stream the Command object.
    *
    * @param os The output stream where the Command object will be streamed.
    * @param cmd The Command object to be streamed.
    * @return A reference to the output stream after streaming the Command object.
    */
    friend std::ostream &operator<<(std::ostream &os, const CommandBase &cmd);
};

/* 
 *  Derived from Command. Implements its abstract methods.       
 *                                                               
 *  Consists of operands and operator acting on it.                                                                 
 */

class Command : public CommandBase {


friend ExecUnit;

    my_std::Vec<std::unique_ptr<Operand>> opds;
    Operator oper;

protected:

    /**
    * @brief Executes the command operation in the cache.
    * 
    * @param cache The cache object where the operator is executed.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void exec_in_cache (CPU_Cache &cache) const override;

    /**
    * @brief Loads command from CPU to cache.
    * 
    * @param cache The cache object to which command's operands are loaded.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void load_to_cache (CPU_Cache &cache) const override;

    /**
    * @brief Prints command contents.
    * @param os The output stream for printing.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void print (std::ostream &os) const override;

public:

    /**
    * @brief Destructor for Command class
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    ~Command() noexcept override {}

    /**
    * @brief Default move constructor for Command class
    * 
    * @param other Rvalue reference to another Command object
    */
    Command (Command &&other) = default;

    /**
    * @brief Creates a copy of the Command object
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    * 
    * @return Pointer to the cloned Command object
    */
    Command* clone () const override;

    /**
    * @brief Constructor for Command class
    * 
    * @param _lbl ID of the command label
    * @param _oper Operator to be applied to operand
    * @param _opd1 Unique pointer to the first operand
    */
    Command(const ID _lbl, const Operator _oper, my_std::Vec<std::unique_ptr<Operand>> _opds);

    /**
    * @brief Constructor for Command class
    * 
    * @param _oper Operator to be applied to operand
    * @param _opd1 Unique pointer to the first operand
    */
    Command(const Operator _oper, my_std::Vec<std::unique_ptr<Operand>> _opds);

};

