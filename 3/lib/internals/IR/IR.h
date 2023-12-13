/** @file IR/IR.h
 *  
 *  Arch-independent basic classes of IR.
 */

#pragma once
#include "fwd_IR.h"


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  The label class.                                             *
 *                                                               *
 *  ID string must consist of up to 8 latin letters.             *
 *                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct ID {

    std::string id;
    addr_t addr;
    bool is_ascii = 0;

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
    * @brief Trivial accessor for ID.
    *
    * @return The ID string.
    */
    const std::string &get_id () const { return id; }
    
    /**
    * @brief Trivial accessor for ID address.
    *
    * @return The ID address.
    */
    addr_t get_addr () const { return addr; }

    /**
    * @brief Trivial accessor for being ascii.
    *
    * @return Is ascii.
    */
    bool iSascii() const { return is_ascii; }

    /** 
    * @brief Trivial setter for ID value.
    */
    void set_addr (addr_t _addr) { addr = _addr; }

    /** 
    * @brief Trivial setter for being ascii.
    */
    void set_ascii (bool is_ascii) { this->is_ascii = is_ascii; }

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
    bool operator== (const ID& id) const { return id.get_id() == get_id();}
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


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  The base abstract class of all operands.                     *
 *                                                               *
 *  Contains value, is an immediate operand at its essence.      *
 *  Also contains some general interface.                        *
 *  Can be loaded from CPU and back.                             * 
 *                                                               *
 *  Compiler recognize GPRegisters, SPRegisters, DataCells       *
 *  and ImmOperands.                                             *
 *                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Operand {

friend UnaryCommand;
friend BinaryCommand;
friend TernaryCommand;
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
friend UnaryCommand;
friend BinaryCommand;
friend TernaryCommand;
friend CPU;

    std::unique_ptr<Operand> opd1 = nullptr;
    std::unique_ptr<Operand> opd2 = nullptr;
    std::unique_ptr<Operand> opd3 = nullptr;
    CPU *cpu = nullptr;
    CPU_Cache (CPU* _parent) : cpu(_parent) {};
    CPU_Cache (const CPU_Cache &c);

    void load_opd1 (Operand &_opd1);
    void load_opd2 (Operand &_opd2);
    void load_opd3 (Operand &_opd3);

    void clear ();
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *  The base abstract class of all operators.                    *
 *                                                               *
 *  Operator consists of operation, time of its execution        *
 *  and their assembler mnemonics (parsed by compiler).          *
 *  Compiler recognize only unary and binary operators.          *
 *  Any instruction sets are formed from them.                   *
 *                                                               *
 *  Result of an operation is always put to first operand.       * 
 *                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class Operator {

protected:
    Mnemonic mnem;
    short time;

public:
    Operator () {};

    /**
    * @brief Constructor for the Operator class.
    *
    * @param _mnem The mnemonic string for the Operator.
    */
    Operator(const Mnemonic _mnem) { mnem = _mnem; }

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
std::ostream &operator<<(std::ostream &os, const Operator &op);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 *  Derived from Operator.                                       *
 *                                                               *
 *  Consists of unary operation acting on singular operand.      *
 *                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

    /**
    * @brief Constructor for the UnaryOperator class.
    *
    * @param _mnem The mnemonic string for the UnaryOperator.
    */
    UnaryOperator(const Mnemonic &_mnem) : Operator(_mnem) {}

    /**
    * @brief Function call operator for UnaryOperator.
    *
    * @param opd1 The first operand reference.
    */
    void operator() (Operand &opd1) const { oper(opd1); }

    /**
    * @brief Equality comparison operator for UnaryOperator.
    *
    * @param other The UnaryOperator object to compare with.
    * @return True if the mnemonics are the same, false otherwise.
    */    
    bool operator== (const UnaryOperator& other) const noexcept;
};

/*
 *  Derived from Operator.                                       
 *                                                               
 *  Consists of binary operation acting on two operands.         
 *                                                               
 */

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

    /**
    * @brief Constructor for the BinaryOperator class.
    *
    * @param _mnem The mnemonic string for the BinaryOperator.
    */
    BinaryOperator(const Mnemonic &_mnem) : Operator(_mnem) {}
    
    /**
    * @brief Function call operator for BinaryOperator.
    *
    * @param opd1 The first operand reference.
    * @param opd2 The second operand reference.
    */
    void operator() (Operand &opd1, Operand &opd2) const { oper(opd1, opd2); }
    
    /**
    * @brief Equality comparison operator for BinaryOperator.
    *
    * @param other The BinaryOperator object to compare with.
    * @return True if the mnemonics are the same, false otherwise.
    */    
    bool operator== (const BinaryOperator& other) const noexcept;
};   

/* 
 *  Derived from Operator.                                       
 *                                                               
 *  Consists of ternary operation acting on three operands.         
 *                                                               
 */

class TernaryOperator : public Operator {

protected:
    void (*oper)(Operand &opd1, Operand &opd2, Operand &opd3);

public:

    TernaryOperator () {};

    TernaryOperator(TernaryOperator &copy) = default;
    TernaryOperator(const TernaryOperator &copy) = default;
    TernaryOperator &operator= (TernaryOperator &copy) = default;

    TernaryOperator(TernaryOperator &&move) = default;
    TernaryOperator &operator= (TernaryOperator &&move) = default;

    /**
    * @brief Constructor for the TernaryOperator class.
    *
    * @param _mnem The mnemonic string for the TernaryOperator.
    */
    TernaryOperator(const Mnemonic &_mnem) : Operator(_mnem) {};
    
    /**
    * @brief Function call operator for TernaryOperator.
    *
    * @param opd1 The first operand reference.
    * @param opd2 The second operand reference.
    * @param opd3 The third operand reference.
    */
    void operator() (Operand &opd1, Operand &opd2, Operand &opd3) const { oper(opd1, opd2, opd3); } 
    
    /**
    * @brief Equality comparison operator for TernaryOperator.
    *
    * @param other The TernaryOperator object to compare with.
    * @return True if the mnemonics are the same, false otherwise.
    */    
    bool operator== (const TernaryOperator& other) const noexcept;
};   


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

    template<> struct hash<TernaryOperator>
    {
        std::size_t operator()(const TernaryOperator& oper) const noexcept
        {
            std::size_t hash = std::hash<Mnemonic>()(oper.mnemonics());
            return hash;
        }
    };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Contains a table of operators and its mnemonics.              *
 *                                                               *
 * By design, user instruction set links as a separate library.  *
 * User must implement own tables for custom unary and binary    *
 * operators and construct own instruction set from them.        *                                                         
 *                                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using unary_instr_set = std::unordered_set<UnaryOperator>;
using binary_instr_set = std::unordered_set<BinaryOperator>;
using ternary_instr_set = std::unordered_set<TernaryOperator>;

class InstrSet {
    const unary_instr_set uset;
    const binary_instr_set bset;
    const ternary_instr_set tset;

public:

    /**
    * @brief Constructor for the InstrSet class.
    *
    * @param _uset A const reference to unary_instr_set object.
    * @param _bset A const reference to binary_instr_set object.
    */
    InstrSet(const unary_instr_set& _uset, const binary_instr_set& _bset, const ternary_instr_set& _tset);

    /**
    * @brief Finds an unary operator by its mnemonics.
    *
    * @param str A const reference to mnemonic string of the unary operator.
    * @throw std::logic_error "Unary operator not found"
    * @return A reference to the UnaryOperator object.
    */
    UnaryOperator &FindUnOper(const Mnemonic &str) const;

    /**
    * @brief Finds a binary operator by its mnemonics.
    *
    * @param str A const reference to mnemonic string of the binary operator.
    * @throw std::logic_error "Binary operator not found"
    * @return A reference to the BinaryOperator object.
    */
    BinaryOperator &FindBinOper(const Mnemonic &str) const;

     /**
    * @brief Finds a ternary operator by its mnemonics.
    *
    * @param str A const reference to mnemonic string of the ternary operator.
    * @throw std::logic_error "Ternary operator not found"
    * @return A reference to the TernaryOperator object.
    */
    TernaryOperator &FindTernOper(const Mnemonic &str) const;
};


/* 
 *  The base abstract class of all commands.                     
 *                                                               
 *  Contains label and some general interface.                   
 *  Consists of operands and operation on them.                  
 *  Is always executed in cache.                                 
 *  Result of an operation is always put to first operand.        
 *                                                               
 *  Compiler recognize only unary and binary commands.           
 *                                                               
 */

class Command {

friend ExecUnit;

protected:
    ID lbl = "";

    /**
    * @brief Executes the command operation in the cache.
    * 
    * @param cache The cache object where the command operation is executed.
    *
    * @note This is a pure virtual function.
    * @see UnaryCommand, BinaryCommand
    */
    virtual void exec_in_cache (CPU_Cache &cache) const = 0;

    /**
    * @brief Loads command operands from CPU to cache.
    * 
    * @param cache The cache object to which command's operands are loaded.
    *
    * @note This is a pure virtual function.
    * @see UnaryCommand, BinaryCommand
    */
    virtual void load_to_cache (CPU_Cache &cache) const = 0;

    /**
    * @note This is a pure virtual function.
    * @see UnaryCommand, BinaryCommand
    */
    virtual void print (std::ostream &os) const = 0;

public:

    /**
    * @note This is a pure virtual function.
    * @see UnaryCommand, BinaryCommand
    */
    virtual ~Command() = default;

    /**
    * @note This is a pure virtual function.
    * @see UnaryCommand, BinaryCommand
    */
    virtual Command* clone () const = 0;

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
    friend std::ostream &operator<<(std::ostream &os, const Command &cmd);
};


/* 
 *  Derived from Command. Implements its abstract methods.       
 *                                                               
 *  Consists of operand and unary operator acting on it.         
 *                                                               
 */

class UnaryCommand : public Command {

    std::unique_ptr<Operand> opd1;
    UnaryOperator unoper;

protected:

    /**
    * @brief Executes the unary command operation in the cache.
    * 
    * @param cache The cache object where the unary operator is executed.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void exec_in_cache (CPU_Cache &cache) const override;

    /**
    * @brief Loads unary command from CPU to cache.
    * 
    * @param cache The cache object to which command's operands are loaded.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void load_to_cache (CPU_Cache &cache) const override;

    /**
    * @brief Prints unary command contents.
    * @param os The output stream for printing.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void print (std::ostream &os) const override;

public:

    /**
    * @brief Destructor for UnaryCommand class
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    ~UnaryCommand() noexcept override {}

    /**
    * @brief Default move constructor for UnaryCommand class
    * 
    * @param other Rvalue reference to another UnaryCommand object
    */
    UnaryCommand (UnaryCommand &&other) = default;

    /**
    * @brief Creates a copy of the BinaryCommand object
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    * 
    * @return Pointer to the cloned BinaryCommand object
    */
    UnaryCommand* clone () const override;

    /**
    * @brief Constructor for UnaryCommand class
    * 
    * @param _lbl ID of the command label
    * @param _oper UnaryOperator to be applied to operand
    * @param _opd1 Unique pointer to the first operand
    */
    UnaryCommand(const ID _lbl, const UnaryOperator _oper, std::unique_ptr<Operand> _opd1);

    /**
    * @brief Constructor for UnaryCommand class
    * 
    * @param _oper UnaryOperator to be applied to operand
    * @param _opd1 Unique pointer to the first operand
    */
    UnaryCommand(const UnaryOperator _oper, std::unique_ptr<Operand> _opd1);

};

/* 
 *  Derived from Command. Implements its abstract methods.       
 *                                                               
 *  Consists of two operands and binary operator acting on them. 
 *                                                               
 */

class BinaryCommand : public Command {
    
    std::unique_ptr<Operand> opd1;
    std::unique_ptr<Operand> opd2;
    BinaryOperator binoper;

protected:

    /**
    * @brief Executes the binary command operation in the cache.
    * 
    * @param cache The cache object where the binary operator is executed.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void exec_in_cache (CPU_Cache &cache) const override;
    
    /**
    * @brief Loads binary command from CPU to cache.
    * 
    * @param cache The cache object to which command's operands are loaded.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void load_to_cache (CPU_Cache &cache) const override;

    /**
    * @brief Prints binary command contents.
    * @param os The output stream for printing.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void print (std::ostream &os) const override;

public:

    /**
    * @brief Destructor for BinaryCommand class
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    ~BinaryCommand() noexcept override {}

    /**
    * @brief Default move constructor for BinaryCommand class
    * 
    * @param other Rvalue reference to another BinaryCommand object
    */
    BinaryCommand (BinaryCommand &&other) = default;

    /**
    * @brief Creates a copy of the BinaryCommand object
    * 
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    *
    * @return Pointer to the cloned BinaryCommand object
    */
    BinaryCommand* clone () const override;

    /**
    * @brief Constructor for BinaryCommand class
    * 
    * @param _lbl ID of the command label
    * @param _oper BinaryOperator to be applied to operands
    * @param _opd1 Unique pointer to the first operand
    * @param _opd2 Unique pointer to the second operand
    */
    BinaryCommand(const ID _lbl, const BinaryOperator _oper, 
        std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2);

    /**
    * @brief Constructor for BinaryCommand class
    * 
    * @param _oper BinaryOperator to be applied to operands
    * @param _opd1 Unique pointer to the first operand
    * @param _opd2 Unique pointer to the second operand
    */
    BinaryCommand(const BinaryOperator _oper, 
        std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2);

};

/* 
 *  Derived from Command. Implements its abstract methods.          
 *                                                                  
 *  Consists of three operands and ternary operator acting on them. 
 *                                                                  
 */

class TernaryCommand : public Command {
    
    std::unique_ptr<Operand> opd1;
    std::unique_ptr<Operand> opd2;
    std::unique_ptr<Operand> opd3;
    TernaryOperator ternoper;

protected:

    /**
    * @brief Executes the ternary command operation in the cache.
    * 
    * @param cache The cache object where the ternary operator is executed.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void exec_in_cache (CPU_Cache &cache) const override;
    
    /**
    * @brief Loads ternary command from CPU to cache.
    * 
    * @param cache The cache object to which command's operands are loaded.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void load_to_cache (CPU_Cache &cache) const override;

    /**
    * @brief Prints ternary command contents.
    * @param os The output stream for printing.
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    void print (std::ostream &os) const override;

public:

    /**
    * @brief Destructor for TernaryCommand class
    *
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    */
    ~TernaryCommand() noexcept override {};

    /**
    * @brief Default move constructor for TernaryCommand class
    * 
    * @param other Rvalue reference to another TernaryCommand object
    */
    TernaryCommand (TernaryCommand &&other) = default;

    /**
    * @brief Creates a copy of the TernaryCommand object
    * 
    * @note This is an implementation of the abstract one in base Command class.
    * @see Command
    *
    * @return Pointer to the cloned TernaryCommand object
    */
    TernaryCommand* clone () const override;

    /**
    * @brief Constructor for TernaryCommand class
    * 
    * @param _lbl ID of the command label
    * @param _oper TernaryOperator to be applied to operands
    * @param _opd1 Unique pointer to the first operand
    * @param _opd2 Unique pointer to the second operand
    * @param _opd3 Unique pointer to the third operand
    */
    TernaryCommand(const ID _lbl, const TernaryOperator _oper, 
        std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2, std::unique_ptr<Operand> _opd3);

    /**
    * @brief Constructor for TernaryCommand class
    * 
    * @param _oper TernaryOperator to be applied to operands
    * @param _opd1 Unique pointer to the first operand
    * @param _opd2 Unique pointer to the second operand
    * @param _opd3 Unique pointer to the third operand
    */
    TernaryCommand(const TernaryOperator _oper, 
        std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2, std::unique_ptr<Operand> _opd3);

};

