#pragma once
#include "../cpu/cpu.h"



class Mem_Register : public Register {
    void load(CPU &cpu) const;
};



