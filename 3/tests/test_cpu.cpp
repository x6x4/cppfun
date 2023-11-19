
#include <ostream>
#define CATCH_CONFIG_MAIN

#include <sstream>
#include "../instr_set/instr_set.h"
#include "../lib/IR_compiler/fwd_IR_compiler.h"
#include <catch2/catch.hpp>

std::stringstream out;

class CLI_DBG {

CPU *cpu;

public:

    CLI_DBG (CPU *_cpu) : cpu(_cpu) {};

    void operator() () { 
        cpu->print_gpregblock(out); 
        cpu->print_dm(out); 
        cpu->print_spregblock(out); 
    };
};

TEST_CASE("SIMPLE CASES") {

    CPU cpu (iset);
    // store a call to a function object
    std::function<void()> f = CLI_DBG(&cpu);

    const char *prog1 = 
    "label: inc %r7";

    load_text_cpu(cpu, prog1);
    my_std::Vec <std::size_t> bps = {};    
    exec(cpu, bps, f);

    REQUIRE(out.str() == "r0(0) r1(0) r2(0) r3(0) r4(0) r5(0) r6(0) r7(1) \n0 0 0 0 0 0 0 0 \npc(1) zf(0) \n");
    out.str("");

    const char *prog2 = 
    "inc %r2\nmov %r3 %r2";

    load_text_cpu(cpu, prog2);
    bps = {};        
    exec(cpu, bps, f);

    REQUIRE(out.str() == "r0(0) r1(0) r2(1) r3(1) r4(0) r5(0) r6(0) r7(0) \n0 0 0 0 0 0 0 0 \npc(2) zf(0) \n");
    out.str("");

    load_file_cpu(cpu, "/home/cracky/cppfun/3/prog2.asm");
    bps = {5};        
    exec(cpu, bps, f);
    REQUIRE(out.str() == "r0(0) r1(0) r2(1) r3(5) r4(1) r5(0) r6(1) r7(0) \n5 7 0 0 0 0 0 0 \npc(5) zf(0) \n"
    "r0(0) r1(0) r2(1) r3(5) r4(1) r5(0) r6(1) r7(0) \n5 1 0 0 0 0 0 0 \npc(6) zf(0) \n");
    out.str("");
}