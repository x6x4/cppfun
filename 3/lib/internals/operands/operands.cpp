
#include "operands.h"

//  REG  //

//  gp

void GPRegister::print (std::ostream &os) const { os << "%r" << num; }

void GPRegister::load_to(CPU &cpu) const { cpu.gp_rb.load_reg(num, value); }

void GPRegister::load_from(CPU &cpu) { value = cpu.gp_rb[num]; }

std::unique_ptr<Operand> GPRegister::clone () const { return std::make_unique<GPRegister>(*this); }

//  sp

void PCRegister::print (std::ostream &os) const { }

void PCRegister::load_to(CPU &cpu) const { cpu.mem.pm.set_spreg(num, value); }

void PCRegister::load_from(CPU &cpu) {}

std::unique_ptr<Operand> PCRegister::clone () const { return std::make_unique<PCRegister>(*this); }

//  dc

void DataCell::print (std::ostream &os) const { os << '(' << num << ')'; }

void DataCell::load_to(CPU &cpu) const { cpu.mem.dm.data[num] = value; }

void DataCell::load_from(CPU &cpu) { value = cpu.mem.dm.data[num]; }

std::unique_ptr<Operand> DataCell::clone () const { return std::make_unique<DataCell>(*this); }
