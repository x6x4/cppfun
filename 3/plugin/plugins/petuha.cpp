#include <string>
#include "../../lib/internals/operands/operands.h"
#include "../plugin_internals/api/include/plugin.h"
#include "../plugin_internals/api/include/plugin_manager.h"


class OperBinaryPower : public Operator {

private:
  long long binpow(long long n, unsigned long long m) const {
    if (m == 0) return 1;
    if (m % 2 == 0) {
      long long temp = binpow(n, m / 2);
      return temp * temp;
    }
    return binpow(n, m - 1) * n;
  }

public:
  OperBinaryPower() : Operator("binpow") {}
  void oper (my_std::Vec<std::unique_ptr<Operand>> &opds) const override {
    if (opds.size() != 2) throw std::invalid_argument("Incorrect operands");
    if (opds[1]->val() < 0) throw std::logic_error("Power can not be negative");
    opds[0]->set(binpow(opds[0]->val(), opds[1]->val()));
  }
};

class OperBinaryPowerRemainder : public Operator {
private:
  long long binpowremainder(long long n, unsigned long long m, long long c) const {
    if (m == 0) return 1;
    if (m % 2 == 0) {
      unsigned long long temp = binpowremainder(n, m / 2, c);
      return (temp * temp) % c;
    }
    return (binpowremainder(n, m - 1, c) * n) % c;
  }
public:
  OperBinaryPowerRemainder() : Operator("binpowrem") {}
  void oper (my_std::Vec<std::unique_ptr<Operand>> &opds) const override {
    if (opds.size() != 3) throw std::invalid_argument("Incorrect operands");
    if (opds[1]->val() < 0) throw std::logic_error("Power can not be negative");
    if (opds[2]->val() == 0) throw std::runtime_error("Division by zero");
    if (opds[2]->val() == 1) opds[0]->set(0);
    else opds[0]->set(binpowremainder(opds[0]->val(), opds[1]->val(), opds[2]->val()));
  }
};
class PetuhaPlugin: public InstructionSetPlugin {
  InstrSet iset {{std::make_shared<OperBinaryPower>(OperBinaryPower()), 
  std::make_shared<OperBinaryPowerRemainder>(OperBinaryPowerRemainder())}};
public:
  std::string getName() const override {
    return "Plugin: Operators Binary Power, Binary Power Remainder";
  }
   const InstrSet &getInstrSet() const override {
    return iset;
  }
};
static PetuhaPlugin plugin;
static auto token = PluginToken(plugin);