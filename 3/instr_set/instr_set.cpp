
#include "instr_set.h"
#include <cstddef>
#include <cstdio>
#include <iterator>
#include <stdexcept>
#include <thread>



InstrSet iset {{Oper_Inc(), Oper_Jmp(), Oper_Mov(), Oper_SysCall(), Oper_FindSym(), Oper_CmpStr()}};


//  UNARY OPERATORS

void Oper_Increment (my_std::Vec<std::unique_ptr<Operand>> &opds) {
    auto &opd1 = *opds.at(0);
    opd1.set(opd1.val() + 1);
};

void Oper_Jump (my_std::Vec<std::unique_ptr<Operand>> &opds) {
    auto &opd1 = *opds.at(0);
    if (typeid(opd1) == typeid(PCRegister&))
        static_cast<PCRegister&>(opd1).set_num(0);
    else 
        throw std::logic_error ("Runtime error: jmp instruction requires code label operand");
}

enum SYSCALLS {
    PRINT_STRING = 1
};

//  BINARY OPERATORS

void Oper_Movement (my_std::Vec<std::unique_ptr<Operand>> &opds) {
    auto &opd1 = *opds.at(0);
    auto &opd2 = *opds.at(1);
    opd1.set(opd2.val());
};

void Oper_Syscall (my_std::Vec<std::unique_ptr<Operand>> &opds) {
    auto &opd1 = *opds.at(0);
    auto &opd2 = *opds.at(1);
    if (opd1.val() == SYSCALLS::PRINT_STRING) {
        auto str = static_cast<String&>(opd2);
        std::cout << str;
    }
}

//  TERNARY OPERATORS

int find_sym (const my_std::Vec<int> &str, char sym, size_t init_pos, size_t final_pos) {
    int sym_pos = -1;

    for (auto i = str.begin()+init_pos; i != str.begin() + final_pos; i++) {
        if (*i == sym) {
            sym_pos = *i; break;
        }
    }

    return sym_pos;
}

int find_sym_mt (const my_std::Vec<int> &str, char sym, size_t init_pos, size_t final_pos) {
    std::size_t chars_num = final_pos - init_pos;
    size_t thread_prelim_num = std::thread::hardware_concurrency();
    auto thread_num = std::min(thread_prelim_num, chars_num);
    my_std::Vec<int> results(thread_num);
    my_std::Vec<std::thread> threads(thread_num);

    for (size_t i = 0; i < thread_num; i++) {
        size_t start_i = i*chars_num/thread_num;
        size_t end_i = start_i + chars_num/thread_num;
        auto start = init_pos + start_i;
        auto end = init_pos + end_i;
        threads[i] = std::thread( [=, &results]() {
            results[i] = find_sym(str, sym, start, end) - init_pos + 1;
        } );
    }

    for (auto &th : threads) th.join();

    for (auto r : results) { if (r > 0) return r; }
    return -1;
}

void Oper_FindSymbol (my_std::Vec<std::unique_ptr<Operand>> &opds) {
    auto &opd1 = *opds.at(0);
    auto &opd2 = *opds.at(1);
    auto &opd3 = *opds.at(2);

    if (typeid(opd3) == typeid(String)) {
        auto str = static_cast<String&>(opd3);
        auto begin = std::chrono::steady_clock::now();
        opd1.set(find_sym(str.getStore(), opd2.val(), str.begin(), str.end()));
        auto end = std::chrono::steady_clock::now();

        auto begin_mt = std::chrono::steady_clock::now();
        opd1.set(find_sym_mt(str.getStore(), opd2.val(), str.begin(), str.end()));
        auto end_mt = std::chrono::steady_clock::now();

        std::cout << "Regular: " << std::chrono::duration_cast<std::chrono::microseconds>
        (end - begin).count() << std::endl;
        std::cout << "Multithreaded: " << std::chrono::duration_cast<std::chrono::microseconds>
        (end_mt - begin_mt).count() << std::endl;
    }
    else throw std::logic_error(
    "Runtime error: fsym instruction requires ascii data label operand");
}


void Oper_CompareStrings (my_std::Vec<std::unique_ptr<Operand>> &opds) {
    /*auto &opd1 = *opds.at(0);
    auto &opd2 = *opds.at(1);
    auto &opd3 = *opds.at(2);*/
}

//  parallel programming 
//  Oper_Strcmp %r1/%r2 scmpsb
//  scasb - find symbol in string 


