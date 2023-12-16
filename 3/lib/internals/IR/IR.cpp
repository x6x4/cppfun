
#include "IR.h"
#include <cstddef>
#include <ios>
#include <iostream>


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

std::ostream &operator<<(std::ostream &os, const ID &_id) {
    os << _id.id;
    return os;
}

//  OPERAND  //

std::ostream &operator<<(std::ostream &os, const Operand &opd) {
    opd.print(os);
    return os;
}

//  CACHE  //

CPU_Cache::CPU_Cache (const CPU_Cache &c) {
    for (size_t cur = 0; cur < opds.size(); cur++)
        if (opds[cur]) opds[cur] = c.opds[cur]->clone();
};

void CPU_Cache::load_opds (const my_std::Vec<std::unique_ptr<Operand>> &_opds) {
    for (auto &opd : _opds) {
        opd->load_from(*cpu);
        opds.push_back(opd->clone());
    }
};

void CPU_Cache::clear () { 
    for (auto &opd : opds) opd = nullptr; 
    opds.clear();
}

//  OPERATOR  //

std::ostream &operator<<(std::ostream &os, const OperatorBase &op) {
    os << op.mnemonics();
    return os;
}

bool Operator::operator== (const Operator &other) const noexcept{
    return mnemonics() == other.mnemonics();
}


//  INSTR SET  //

Operator &InstrSet::FindOper (const Mnemonic &str) const {
    auto oper = iset.find(Operator(str));
    if (oper == iset.end())
        throw std::logic_error("Operator not found");
    else 
        return oper._M_cur->_M_v();
};


//  COMMAND BASE  //

std::ostream &operator<<(std::ostream &os, const CommandBase &cmd) {
    cmd.print(os);
    return os;
}

//  COMMAND

void Command::load_to_cache (CPU_Cache &cache) const
    { cache.load_opds(opds); }

void Command::exec_in_cache (CPU_Cache &cache) const
    { load_to_cache(cache); oper(cache.opds); }

void Command::print(std::ostream &os) const {
    os << oper.mnemonics();
    for (const auto &opd : opds) os << " " << *opd;
    os << '\n';
}

Command* Command::clone () const {
    my_std::Vec<std::unique_ptr<Operand>> _opds; 
    for (const auto &opd : opds) _opds.push_back(opd->clone());
    return new Command(lbl, oper, std::move(_opds)); 
}

Command::Command(const ID _lbl, const Operator _oper, 
    my_std::Vec<std::unique_ptr<Operand>> _opds) 
    : opds(std::move(_opds)), oper(_oper) { CommandBase::lbl = _lbl; }

Command::Command(const Operator _oper, 
    my_std::Vec<std::unique_ptr<Operand>> _opds) 
    : opds(std::move(_opds)), oper(_oper) {}


