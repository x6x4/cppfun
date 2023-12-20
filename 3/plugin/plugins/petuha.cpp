
#include <string>

#include "../../lib/internals/operands/operands.h"
#include "../plugin_internals/api/include/plugin.h"
#include "../plugin_internals/api/include/plugin_manager.h"


class Oper_Petuha : public Operator {
public:
    Oper_Petuha() : Operator("petuha") {}

    void oper (my_std::Vec<std::unique_ptr<Operand>> &opds) const override {

    }
};


class PetuhaPlugin: public InstructionSetPlugin {

  InstrSet iset {{std::make_shared<Oper_Petuha>(Oper_Petuha())}};

public:

  std::string getName() const override {
    return "Plugin: Operators Petuha";
  }
  
  const InstrSet &getInstrSet() const override {
    return iset;
  }
};

static PetuhaPlugin plugin;
static auto token = PluginToken(plugin);