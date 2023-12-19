#include <string>
#include <chrono>
#include "fmt/chrono.h"

#include "../plugin_internals/api/include/plugin.h"
#include "../plugin_internals/api/include/plugin_manager.h"


void Oper_Increment (my_std::Vec<std::unique_ptr<Operand>> &opds) {
    auto &opd1 = *opds.at(0);
    opd1.set(opd1.val() + 1);
};

class Oper_Inc : public Operator {
public:
    Oper_Inc() : Operator("inc") { oper = Oper_Increment; }
};


class DefaultSetPlugin: public InstructionSetPlugin {

  InstrSet iset {{Oper_Inc()}};

public:

  std::string getName() const override {
    return "Plugin: Operator Increment";
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
