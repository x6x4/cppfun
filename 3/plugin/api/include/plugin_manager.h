#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include "plugin.h"

#include <unordered_map>
#include <stdexcept>
#include <fmt/core.h>

class PluginManager {

  public:

  void addPlugin(const OperatorPlugin& plugin);

  void removePlugin(const std::string& name);

  const std::unordered_map<std::string, std::reference_wrapper<const OperatorPlugin>>& getPlugins();

  static PluginManager& getInstance();

  private:

  std::unordered_map<std::string, std::reference_wrapper<const OperatorPlugin>> plugins;

  PluginManager() = default;

};

class PluginToken {

  public:

  PluginToken(const OperatorPlugin& plugin);

  ~PluginToken();

  private:

  const OperatorPlugin& plugin;
};

#endif // PLUGIN_MANAGER_H
