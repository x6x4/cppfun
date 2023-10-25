
#include "cpu.h"
#include <iostream>
#include <iterator>
#include <stdexcept>


//  REG  //

//  gp

void GPRegister::print (std::ostream &os) const { os << "%r" << num << '(' << val() << ')'; }

void GPRegister::load_to(CPU &cpu) const { cpu.gp_rb.load_reg(num, value); }

void GPRegister::load_from(CPU &cpu) { value = cpu.gp_rb[num]; }

std::unique_ptr<Operand> GPRegister::clone () const { return std::make_unique<GPRegister>(*this); }

//  sp

void SPRegister::print (std::ostream &os) const { os << '(' << val() << ')'; }

void SPRegister::load_to(CPU &cpu) const { cpu.mem.pm.set_spreg(num, value); }

void SPRegister::load_from(CPU &cpu) {}

std::unique_ptr<Operand> SPRegister::clone () const { return std::make_unique<SPRegister>(*this); }

//  dc

void DataCell::print (std::ostream &os) const { os << '(' << val() << ')'; }

void DataCell::load_to(CPU &cpu) const { cpu.mem.dm.data[num] = value; }

void DataCell::load_from(CPU &cpu) { value = cpu.mem.dm.data[num]; }

std::unique_ptr<Operand> DataCell::clone () const { return std::make_unique<DataCell>(*this); }

//  rb

int RegBlock::operator[] (std::size_t num) const { 
    if (num < regs.size()) return regs[num]; 
    throw std::logic_error("Wrong register number");
}

void RegBlock::load_reg (std::size_t num, int val) { regs[num] = val; };

void RegBlock::print (std::ostream &os) const { 
    for (std::size_t i = 0; i < regs.size(); i++) 
        if (regs[i]) os << 'r' << i << '(' << regs[i] << ')' << ' '; 
    os << '\n';
}

std::ostream &operator<<(std::ostream &os, RegBlock &rb) {
    rb.print(os);
    return os;
}

//  CPU  //

bool CPU::CPU_exists = 0;

void CPU::check_existence () {
    if (CPU_exists) throw std::logic_error("Can't create more than one CPU");
    CPU_exists = 1;
}

void CPU::exec (Mem &&m) {
    mem.dm.load(*m.first);
    mem.pm.load(*m.second);

    std::cout << "IR:" << std::endl << mem.pm << std::endl;
    std::cout << "Data:" << std::endl << mem.dm << std::endl;

    while (!mem.pm.is_over()) {
        const Command &cur_cmd = mem.pm.fetch();
        assign(cur_cmd);
    }
}

void CPU::assign(const Command &cmd) {
    for (std::size_t i = 0; i < EUs.size(); i++) {
        auto &eu = EUs[i];
        if (eu.first == State::FREE) {
            EUs[i].first = State::BUSY;
            std::cout << "Command assigned to EU " << i << std::endl;
            EUs[i].second.exec(cmd, *this);
            EUs[i].first = State::FREE;
            return;
        }
    }

    std::cout << "Command can't be assigned now, wait please" << std::endl;
}

//  EXEC UNIT  //

void ExecUnit::exec(const Command &cmd, CPU &cpu) const {
    cmd.exec(cpu.cache, cpu);
    cpu.load_from_cache();
    cpu.print_regblock(std::cout);
    cpu.mem.pm.print_regblock(std::cout);

    std::cout << "Data: " << cpu.mem.dm;
    std::cout << std::endl;
}

//  LOAD FROM CACHE  //

void CPU::load_from_cache () {
    if (cache.opd1) cache.opd1->load_to(*this);
    if (cache.opd2) cache.opd2->load_to(*this);
    cache.clear();
}
