//#include "lib/mem/mem.h"
#include "lib/mem/mem.h"
#include "lib/prog/instr_set.h"
#include "lib/prog/prog.h"
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <vector>


using reg = int;

int main (int argc, char **argv) {

    Memory m;

    /*const char *src = argv[1];

    if (!src) {
        std::cout << "This is QASM compiler. Usage: qasm <prog_name>" << std::endl;
        return 0;
    }*/

/*      mov r5 5
        inc r8
        jmp loop
code1:  mov r6 data1
        neg r3
        cmp r5 r8
        jne loop*/

    try {

        auto r = Register(5);
        auto dl = DataLabel(0x10);

        BinaryCommand mov1 {Op_Mov(), r, dl};
        std::cout << dl << std::endl;

        /*UnaryCommand inc {Op_Inc(), Register(8)};

        UnaryCommand jmp {Op_Jmp(), DataLabel(0x2)};

        //BinaryCommand mov {"code1", Op_Mov(), Register(6), "data1"};

        UnaryCommand neg {Op_Neg(), DataLabel(0x0)};

        BinaryCommand cmp {Op_Cmp(), Register(5), Register(8)};

        UnaryCommand jne (Op_Jne(), DataLabel(0x2));*/

        //, &inc, &jmp, &neg, &cmp, &jne};
        m.pm.prog = prog_t{&mov1};

        //std::cout << m.pm;
    //    compile (src, m);

    }

    catch (std::logic_error &e) {
        std::cout << e.what();
    }

    catch (std::runtime_error &e) {
        std::cout << e.what();
    }
    
}

