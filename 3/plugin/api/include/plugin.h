#ifndef PLUGIN_H
#define PLUGIN_H

#include <string>

class OperatorPlugin {
public:
  virtual std::string getName() const = 0;
  virtual std::string getVersion() const = 0;
  virtual std::string getMsg() const = 0;
  virtual ~OperatorPlugin() = default;
};

#endif // PLUGIN_H
