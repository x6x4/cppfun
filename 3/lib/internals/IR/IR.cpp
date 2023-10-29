
#include "IR.h"
#include <ios>


//  ID  //

void check_id (std::string name) {
    if (name.size() > 8)
        throw std::logic_error ("ID can't be more than 8 symbols");

    for (char c : name) {
        if (!std::isalpha(c))
            throw std::logic_error ("ID can only contain latin letters");
    }
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

void Cache::load_opd1 (Operand &_opd1, CPU &cpu) {
    opd1 = _opd1.clone();
    opd1->load_from(cpu);
};
void Cache::load_opd2 (Operand &_opd2, CPU &cpu) {
    opd2 = _opd2.clone();
    opd2->load_from(cpu);
};

void Cache::clear () {
    opd1 = nullptr;
    opd2 = nullptr;
}


//  OPERATOR  //

std::ostream &operator<<(std::ostream &os, const Operator &op) {
    os << op.mnemonics();
    return os;
}

bool UnaryOperator::operator== (const UnaryOperator &other) const {
    return mnemonics() == other.mnemonics();
}

bool BinaryOperator::operator== (const BinaryOperator &other) const {
    return mnemonics() == other.mnemonics();
    return 0;
}

//  INSTR SET  //

UnaryOperator &InstrSet::FindUnOper (Mnemonic str) {
    auto uoper = uset.find(UnaryOperator(str));
    if (uoper == uset.end())
        throw std::logic_error("Unary operator not found");
    else 
        return uoper._M_cur->_M_v();
};

BinaryOperator &InstrSet::FindBinOper (Mnemonic str) {
    auto boper = bset.find(BinaryOperator(str));
    if (boper == bset.end())
        throw std::logic_error("Binary operator not found");
    else 
        return boper._M_cur->_M_v();
};

InstrSet::InstrSet(unary_instr_set& _uset, binary_instr_set& _bset) 
: uset(_uset), bset(_bset) {}; 


//  COMMAND  //

std::ostream &operator<<(std::ostream &os, Command &cmd) {
    cmd.print(os);
    return os;
}

//  UNARY COMMAND

void UnaryCommand::load (Cache &cache, CPU &cpu) const{ cache.load_opd1(*opd1, cpu); }

void UnaryCommand::exec (Cache &cache, CPU &cpu) const{ load(cache, cpu); unoper(*cache.opd1); }

void UnaryCommand::print(std::ostream &os) const {
    os << unoper.mnemonics() << " " << *opd1;
}

UnaryCommand* UnaryCommand::clone () const{ return new UnaryCommand(lbl, unoper, opd1->clone()); }

UnaryCommand::UnaryCommand(ID _lbl, UnaryOperator _oper, std::unique_ptr<Operand> _opd1) 
: opd1(std::move(_opd1)), unoper(_oper) {
    Command::lbl = _lbl;
}

UnaryCommand::UnaryCommand(UnaryOperator _oper, std::unique_ptr<Operand> _opd1) 
: opd1(std::move(_opd1)), unoper(_oper) {}

//  BINARY COMMAND

void BinaryCommand::load (Cache &cache, CPU &cpu) const 
{ cache.load_opd1(*opd1, cpu); cache.load_opd2(*opd2, cpu); }

void BinaryCommand::exec (Cache &cache, CPU &cpu) const  
{ load(cache, cpu); binoper(*cache.opd1, *cache.opd2); }

void BinaryCommand::print(std::ostream &os) const {
    os << binoper.mnemonics() << " " << *opd1 << " " << *opd2;
}

BinaryCommand* BinaryCommand::clone () const{ return new BinaryCommand(lbl, binoper, opd1->clone(), opd2->clone()); }

BinaryCommand::BinaryCommand(ID _lbl, BinaryOperator _oper, std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2)
    : opd1(std::move(_opd1)), opd2(std::move(_opd2)), binoper(_oper)  {
    Command::lbl = _lbl;
}

BinaryCommand::BinaryCommand(BinaryOperator _oper, std::unique_ptr<Operand> _opd1, std::unique_ptr<Operand> _opd2) 
    : opd1(std::move(_opd1)), opd2(std::move(_opd2)), binoper(_oper)  {}