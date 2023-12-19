#include "plugin_manager.h"

PluginManager& PluginManager::getInstance() {
  static PluginManager manager;
  return manager;
}

void PluginManager::addPlugin(const InstructionSetPlugin& plugin) {
  auto res = plugins.emplace(plugin.getName(), std::ref(plugin));
  if(!res.second) {
    const InstructionSetPlugin& oldPlugin = res.first->second;
    throw std::runtime_error(
        fmt::format("duplicate plugin: {}:{} and {}:{}",
                    oldPlugin.getName(), oldPlugin.getVersion(),
                    plugin.getName(), plugin.getVersion()));
  }
}

void PluginManager::removePlugin(const std::string& name) {
  plugins.erase(name);
}

const std::unordered_map<std::string, std::reference_wrapper<const InstructionSetPlugin>>& PluginManager::getPlugins() {
  return plugins;
}

PluginToken::PluginToken(const InstructionSetPlugin& plugin): plugin(plugin) {
  PluginManager::getInstance().addPlugin(plugin);
}

PluginToken::~PluginToken() {
  PluginManager::getInstance().removePlugin(plugin.getName());
}
