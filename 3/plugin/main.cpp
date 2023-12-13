#include <filesystem>
#include <string>
#include <vector>
#include <iostream>

#include "api/include/plugin_manager.h"
#include "library_manager/include/library_manager.h"


constexpr auto dlExt = ".so";
constexpr auto searchDir = "/cracky/cppfun/3/build/plugin/plugins/";

int main() {
    LibraryManager libManager;
    libManager.scan(searchDir, dlExt);

    PluginManager& plugManager = PluginManager::getInstance();

    for(const auto& pluginInfo : plugManager.getPlugins()) {
      const OperatorPlugin& plugin = pluginInfo.second;
      std::cout << fmt::format("loaded plugin \"{}\":{}", plugin.getName(), plugin.getVersion())  << std::endl;
    }

    for(const auto& pluginInfo : plugManager.getPlugins()) {
      const OperatorPlugin& plugin = pluginInfo.second;
      std::cout << plugin.getMsg() << std::endl;
    }
}
