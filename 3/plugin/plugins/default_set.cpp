#include <string>

#include "../../lib/internals/operands/operands.h"
#include "../plugin_internals/api/include/plugin.h"
#include "../plugin_internals/api/include/plugin_manager.h"


class Oper_Inc : public Operator {
public:
    Oper_Inc() : Operator("inc") {}

    void oper (my_std::Vec<std::unique_ptr<Operand>> &opds) const override {
        auto &opd1 = *opds.at(0);
        opd1.set(opd1.val() + 1);
    }
};

class Oper_Mov : public Operator {
public:
    Oper_Mov() : Operator("mov") {}

    void oper (my_std::Vec<std::unique_ptr<Operand>> &opds) const override {
        auto &opd1 = *opds.at(0);
        auto &opd2 = *opds.at(1);
        opd1.set(opd2.val());
    }
};

enum SYSCALLS {
    PRINT_STRING = 1
};

class Oper_Syscall : public Operator {
public:
    Oper_Syscall() : Operator("syscall") {}

    void oper (my_std::Vec<std::unique_ptr<Operand>> &opds) const override {
        auto &opd1 = *opds.at(0);
        auto &opd2 = *opds.at(1);
        if (opd1.val() == SYSCALLS::PRINT_STRING) {
            auto str = static_cast<String&>(opd2);
            std::cout << str;
        }
    }
};

class MultithreadPlugin: public InstructionSetPlugin {

  InstrSet iset {{std::make_shared<Oper_Inc>(Oper_Inc()), 
  std::make_shared<Oper_Mov>(Oper_Mov()),
  std::make_shared<Oper_Syscall>(Oper_Syscall())}};

public:

  std::string getName() const override {
    return "Plugin: Operators Inc, Mov, Syscall";
  }

  const InstrSet &getInstrSet() const override {
    return iset;
  }
};

static MultithreadPlugin plugin;
static auto token = PluginToken(plugin);
