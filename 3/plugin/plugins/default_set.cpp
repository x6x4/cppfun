#include <string>
#include <chrono>
#include "fmt/chrono.h"

#include "../plugin_internals/api/include/plugin.h"
#include "../plugin_internals/api/include/plugin_manager.h"


class Oper_Inc : public Operator {
public:
    Oper_Inc() : Operator("inc") {}

    void oper (my_std::Vec<std::unique_ptr<Operand>> &opds) const override {
        std::cout << "here";
        auto &opd1 = *opds.at(0);
        opd1.set(opd1.val() + 1);
    }
};


class DefaultSetPlugin: public InstructionSetPlugin {

  InstrSet iset {{new Oper_Inc()}};

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
