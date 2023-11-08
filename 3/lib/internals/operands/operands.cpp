
#include "operands.h"

//  REG  //

//  gp

void GPRegister::print (std::ostream &os) const { os << "%r" << num; }

void GPRegister::load_to(CPU &cpu) const { cpu.gp_rb.load_reg(num, value); }

void GPRegister::load_from(CPU &cpu) { value = cpu.gp_rb[num]; }

std::unique_ptr<Operand> GPRegister::clone () const { return std::make_unique<GPRegister>(*this); }

//  sp

void SPRegister::print (std::ostream &os) const { }

void SPRegister::load_to(CPU &cpu) const { cpu.mem.pm.set_spreg(num, value); }

void SPRegister::load_from(CPU &cpu) {}

std::unique_ptr<Operand> SPRegister::clone () const { return std::make_unique<SPRegister>(*this); }

//  dc

void DataCell::print (std::ostream &os) const { os << '(' << num << ')'; }

void DataCell::load_to(CPU &cpu) const { cpu.mem.dm.data[num] = value; }

void DataCell::load_from(CPU &cpu) { value = cpu.mem.dm.data[num]; }

std::unique_ptr<Operand> DataCell::clone () const { return std::make_unique<DataCell>(*this); }
