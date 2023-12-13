
#include "instr_set.h"
#include <cstddef>
#include <stdexcept>
#include <thread>

unary_instr_set uset ({Oper_Inc(), Oper_Jmp()}); 
binary_instr_set bset ({Oper_Mov(), Oper_SysCall()});
ternary_instr_set tset ({Oper_FindSym(), Oper_CmpStr()});

InstrSet iset {uset, bset, tset};


//  UNARY OPERATORS

void Oper_Increment (Operand &opd1) {
    opd1.set(opd1.val() + 1);
};

void Oper_Jump (Operand &opd1) {
    if (typeid(opd1) == typeid(PCRegister))
        static_cast<PCRegister&>(opd1).set_num(0);
    else 
        throw std::logic_error ("Runtime error: jmp instruction requires code label operand");
}

enum SYSCALLS {
    PRINT_STRING = 1
};

//  BINARY OPERATORS

void Oper_Movement (Operand &opd1, Operand &opd2) {
    opd1.set(opd2.val());
};

void Oper_Syscall (Operand &opd1, Operand &opd2) {
    if (opd1.val() == SYSCALLS::PRINT_STRING) {
        auto str = static_cast<String&>(opd2);
        std::cout << str;
    }
}

//  TERNARY OPERATORS

void Oper_FindSymbol (Operand &opd1, Operand &opd2, Operand &opd3) {
    int sym_pos = -1;
    char sym = opd2.val();

    if (typeid(opd3) == typeid(String)) {
        auto str = static_cast<String&>(opd3);

        for (size_t i = str.val(); str.getStore().at(i) != '\0' && i != str.getStore().size() - 1; i++) {
            if (str.getStore().at(i) == sym) {
                sym_pos = i - str.val() + 1; break;
            }
        }
    }
    else 
        throw std::logic_error ("Runtime error: fsym instruction requires ascii data label operand");

    opd1.set(sym_pos);
}

/*auto sum_mt(It first, It last) {
    auto threadNum =
    std::thread::hardware_concurrency();
    auto elements = std::distance(first, last);
    std::vector<std::iter_value_t<It>>
    results(threadNum);
    std::vector<std::thread> threads(threadNum);
    for (size_t i = 0; i < threadNum; ++i) {
        size_t start_i = i * elements / threadNum;
        size_t end_i =
        (i + 1) * elements / threadNum;
        auto start = std::next(first, start_i);
        auto end = std::next(first, end_i);
        threads[i] = std::thread([=, &results](){
        results[i] = sum(start, end);
        });
    }

    for(auto& th : threads) th.join();

    return sum(results.begin(), results.end());
}*/

void Oper_CompareStrings (Operand &opd1, Operand &opd2, Operand &opd3) {

}

//  parallel programming 
//  Oper_Strcmp %r1/%r2 scmpsb
//  scasb - find symbol in string 


