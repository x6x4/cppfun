#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "plugin.h"

#include <unordered_map>
#include <stdexcept>
#include <fmt/core.h>

class PluginManager {

  public:

  void addPlugin(const InstructionSetPlugin& plugin);

  void removePlugin(const std::string& name);

  const std::unordered_map<std::string, std::reference_wrapper<const InstructionSetPlugin>>& getPlugins();

  static PluginManager& getInstance();

  private:

  std::unordered_map<std::string, std::reference_wrapper<const InstructionSetPlugin>> plugins;

  PluginManager() = default;

};

class PluginToken {

  public:

  PluginToken(const InstructionSetPlugin& plugin);

  ~PluginToken();

  private:

  const InstructionSetPlugin& plugin;
};

#endif // PLUGIN_MANAGER_H
