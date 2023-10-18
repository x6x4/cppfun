
#include "cmd_base.h"
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

InstrSet::InstrSet(unary_instr_set& _uset, binary_instr_set& _bset) : uset(_uset), bset(_bset) {}; 

//  OPERAND  //

std::ostream &operator<<(std::ostream &os, const Operand &opd) {
    opd.print(os);
    return os;
}

void Register::print (std::ostream &os) const {
    os << "r" << num; //<< " " << val();
}


//  COMMAND  //

std::ostream &operator<<(std::ostream &os, Command &cmd) {
    cmd.print(os);
    return os;
}



//  UNARY COMMAND

void UnaryCommand::load (CPU &cpu) {
    //opd1.load(cpu);
}

void UnaryCommand::exec (CPU &cpu) {
    load(cpu);
    unoper(*opd1);
}

void UnaryCommand::print(std::ostream &os) const {
    os << label() << " " << unoper.mnemonics() << " " << *opd1;
}

//  BINARY COMMAND

void BinaryCommand::load (CPU &cpu) {
    //opd1.load(cpu);
    //opd2.load(cpu);
}

void BinaryCommand::exec (CPU &cpu) {
    load(cpu);
    binoper(*opd1, *opd2);
}

void BinaryCommand::print(std::ostream &os) const {
    os << label() << " " << binoper.mnemonics() << " " << *opd1 << " " << *opd2;
}
