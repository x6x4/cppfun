
#include <cstddef>
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

    void operator() (bpNum) { 
        cpu->print_gpregblock(out); 
        cpu->print_dm(out); 
        cpu->print_spregblock(out); 
    };
};

TEST_CASE("SIMPLE CASES") {

    CPU cpu (iset, 3);
    // store a call to a function object
    std::function<void(bpNum)> f = CLI_DBG(&cpu);

    const char *prog1 = 
    "label: inc %r7";

    my_std::Vec<std::size_t> avl_bps;

    load_text_cpu(cpu, prog1, avl_bps);
    my_std::Vec <bpNum> bps = {};
 
    exec(cpu, bps, f);

    REQUIRE(out.str() == 
    "r0 r1 r2 r3 r4 r5 r6 r7 \n 0  0  0  0  0  0  0  1 \n0 0 0 0 0 0 0 0 \npc(1) zf(0) \n");
    out.str("");

    const char *prog2 = 
    "inc %r2\nmov %r3 %r2";

    load_text_cpu(cpu, prog2, avl_bps);
    bps = {};        
    exec(cpu, bps, f);

    REQUIRE(out.str() == 
    "r0 r1 r2 r3 r4 r5 r6 r7 \n 0  0  1  1  0  0  0  0 \n0 0 0 0 0 0 0 0 \npc(2) zf(0) \n");
    out.str("");

    REQUIRE_THROWS(load_file_cpu(cpu, "/home/cracky/cppfun/3/prog1.asm", avl_bps));

    load_file_cpu(cpu, "/home/cracky/cppfun/3/programs/test.asm", avl_bps);
    bps = {bpNum(5, 0)};        
    exec(cpu, bps, f);
    REQUIRE(out.str() == 
    "r0 r1 r2 r3 r4 r5 r6 r7 \n 0  0  1  5  1  0  1  0 \n5 7 0 0 0 0 0 0 \npc(5) zf(0) \n"
    "r0 r1 r2 r3 r4 r5 r6 r7 \n 0  0  1  5  1  0  1  0 \n5 1 0 0 0 0 0 0 \npc(6) zf(0) \n");
    out.str("");
}