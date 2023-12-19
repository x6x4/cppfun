#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>
#include "../../../../lib/internals/IR/IR.h"

class InstructionSetPlugin {

public:

  virtual std::string getName() const = 0;
  virtual std::string getVersion() const = 0;
  virtual const InstrSet &getInstrSet() const = 0;
  virtual ~InstructionSetPlugin() = default;
};

#endif // PLUGIN_H
