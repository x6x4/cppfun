
#include "operands.h"

//  STRING

void String::print (std::ostream &os) const { 
    for (size_t i = val(); i < store->capacity() && store->at(i) != '\0'; i++) 
        os << char (store->at(i)); 
    os << '\n'; 
}

void String::load_to(CPU &cpu) const { }

void String::load_from(CPU &cpu) { store = &cpu.data(); }

std::unique_ptr<Operand> String::clone () const { return std::make_unique<String>(*this); }

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


//  DATA CELL

//  dc

void DataCell::print (std::ostream &os) const { os << '(' << num << ')'; }

void DataCell::load_to(CPU &cpu) const { cpu.mem.dm.data[num] = value; }

void DataCell::load_from(CPU &cpu) { value = cpu.mem.dm.data[num]; }

std::unique_ptr<Operand> DataCell::clone () const { return std::make_unique<DataCell>(*this); }
