
#include "IR.h"
#include <ios>


//  ID  //

void check_id (const std::string &name) {
    if (name.size() > 8)
        throw std::logic_error ("ID can't be more than 8 symbols");

    for (char c : name) {
        if (!std::isalpha(c))
            throw std::logic_error ("ID can only contain latin letters");
    }
}

ID::ID(const char *str) {
    check_id (str);
    id = str;
}

ID &ID::operator=(ID other) {
    swap(other);
    return *this;
} 

std::istream& operator>> (std::istream& is, ID &id) {
    is >> id.id;
    return is;
}

std::ostream &operator<<(std::ostream &os, const ID &id) {
    os << id.get_id();
    return os;
}

//  OPERAND  //

std::ostream &operator<<(std::ostream &os, const Operand &opd) {
    opd.print(os);
    return os;
}

//  CACHE  //

CPU_Cache::CPU_Cache (const CPU_Cache &c) {
    opd1 = c.opd1->clone();
    opd2 = c.opd1->clone();
};

void CPU_Cache::load_opd1 (Operand &_opd1) {
    opd1 = _opd1.clone();
    opd1->load_from(*cpu);
};

void CPU_Cache::load_opd2 (Operand &_opd2) {
    opd2 = _opd2.clone();
    opd2->load_from(*cpu);
};

void CPU_Cache::clear () {
    opd1 = nullptr;
    opd2 = nullptr;
}

//  OPERATOR  //

std::ostream &operator<<(std::ostream &os, const Operator &op) {
    os << op.mnemonics();
    return os;
}

bool UnaryOperator::operator== (const UnaryOperator &other) const noexcept{
    return mnemonics() == other.mnemonics();
}

bool BinaryOperator::operator== (const BinaryOperator &other) const noexcept{
    return mnemonics() == other.mnemonics();
}

//  INSTR SET  //

UnaryOperator &InstrSet::FindUnOper (const Mnemonic &str) const {
    auto uoper = uset.find(UnaryOperator(str));
    if (uoper == uset.end())
        throw std::logic_error("Unary operator not found");
    else 
        return uoper._M_cur->_M_v();
};

BinaryOperator &InstrSet::FindBinOper (const Mnemonic &str) const {
    auto boper = bset.find(BinaryOperator(str));
    if (boper == bset.end())
        throw std::logic_error("Binary operator not found");
    else 
        return boper._M_cur->_M_v();
};

InstrSet::InstrSet(const unary_instr_set& _uset, 
    const binary_instr_set& _bset) : uset(_uset), bset(_bset) {}; 


//  COMMAND  //

std::ostream &operator<<(std::ostream &os, const Command &cmd) {
    cmd.print(os);
    return os;
}

//  UNARY COMMAND

void UnaryCommand::load_to_cache (CPU_Cache &cache) const
    { cache.load_opd1(*opd1); }

void UnaryCommand::exec_in_cache (CPU_Cache &cache) const
    { load_to_cache(cache); unoper(*cache.opd1); }

void UnaryCommand::print(std::ostream &os) const {
    os << unoper.mnemonics() << " " << *opd1;
}

UnaryCommand* UnaryCommand::clone () const
    { return new UnaryCommand(lbl, unoper, opd1->clone()); }

UnaryCommand::UnaryCommand(const ID _lbl, const UnaryOperator _oper, 
    std::unique_ptr<Operand> _opd1) 
    : opd1(std::move(_opd1)), unoper(_oper) { Command::lbl = _lbl; }

UnaryCommand::UnaryCommand(const UnaryOperator _oper, 
    std::unique_ptr<Operand> _opd1) 
    : opd1(std::move(_opd1)), unoper(_oper) {}

//  BINARY COMMAND

void BinaryCommand::load_to_cache (CPU_Cache &cache) const 
    { cache.load_opd1(*opd1); cache.load_opd2(*opd2); }

void BinaryCommand::exec_in_cache (CPU_Cache &cache) const  
    { load_to_cache(cache); binoper(*cache.opd1, *cache.opd2); }   

void BinaryCommand::print(std::ostream &os) const {
    os << binoper.mnemonics() << " " << *opd1 << " " << *opd2;
}

BinaryCommand* BinaryCommand::clone () const 
    { return new BinaryCommand(lbl, binoper, opd1->clone(), opd2->clone()); }

BinaryCommand::BinaryCommand(const ID _lbl, const BinaryOperator _oper, 
    std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2) 
    : opd1(std::move(_opd1)), opd2(std::move(_opd2)), binoper(_oper) {
    Command::lbl = _lbl;
}

BinaryCommand::BinaryCommand(const BinaryOperator _oper, 
    std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2)  
    : opd1(std::move(_opd1)), opd2(std::move(_opd2)), binoper(_oper) {}