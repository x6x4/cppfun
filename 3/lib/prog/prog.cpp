
#include "prog.h"
#include <ios>


//  ID

std::ostream &operator<<(std::ostream &os, const ID &id) {
    os << id.get_id();
    return os;
}

//  OPERATOR

std::ostream &operator<<(std::ostream &os, const Operator &op) {
    os << op.mnemonics();
    return os;
}

//  OPERAND

std::ostream &operator<<(std::ostream &os, const Operand &opd) {
    opd.print(os);
    return os;
}

void Register::print (std::ostream &os) const {
    os << num << " " << val();
}

void DataLabel::print (std::ostream &os) const {
    os << "0x" << static_cast<int>(label) << " " << val();
}

//  COMMANDS

std::ostream &operator<<(std::ostream &os, Command &cmd) {
    cmd.print(os);
    return os;
}

void UnaryCommand::print(std::ostream &os) const {
    os << label() << " " << op().mnemonics() << " " 
    << opd1;
}

void BinaryCommand::print(std::ostream &os) const {
    os << label() << " " << op().mnemonics() << " " 
    << opd1 << " " << opd2;
}
