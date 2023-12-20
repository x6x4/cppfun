
#include <string>
#include <thread>

#include "../../lib/internals/operands/operands.h"
#include "../plugin_internals/api/include/plugin.h"
#include "../plugin_internals/api/include/plugin_manager.h"


int find_sym (const my_std::Vec<int> &data_mem, char sym, size_t init_pos, size_t final_pos) {
    int sym_pos = -1;

    for (auto i = data_mem.begin()+init_pos; i != data_mem.begin() + final_pos; i++) {
        if (*i == sym) {
            sym_pos = *i; break;
        }
    }

    return sym_pos;
}

int find_sym_mt (const my_std::Vec<int> &data_mem, char sym, size_t init_pos, size_t final_pos) {
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
        threads[i] = std::thread( [=, &data_mem, &results]() {
            results[i] = find_sym(data_mem, sym, start, end) - init_pos + 1;
        } );
    }

    for (auto &th : threads) th.join();

    for (auto r : results) { if (r > 0) return r; }
    return -1;
}


class Oper_FindSymbol : public Operator {
public:
    Oper_FindSymbol() : Operator("fsym") {}

    void oper (my_std::Vec<std::unique_ptr<Operand>> &opds) const override {
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
};

class DefaultSetPlugin: public InstructionSetPlugin {

  InstrSet iset {{std::make_shared<Oper_FindSymbol>(Oper_FindSymbol())}};

public:

  std::string getName() const override {
    return "Plugin: Operators FindSymbol";
  }

  std::string getVersion() const override {
    return "1.0.0";
  }

  const InstrSet &getInstrSet() const override {
    return iset;
  }
};

static DefaultSetPlugin plugin;
static auto token = PluginToken(plugin);
