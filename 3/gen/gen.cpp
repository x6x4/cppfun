
#include "gen.h"

#include <filesystem>
#include <string>
#include <vector>
#include "../plugin/plugin_internals/api/include/plugin_manager.h"


constexpr auto dlExt = ".so";
constexpr auto searchDir = "/home/cracky/cppfun/3/build/plugin/plugins";

InstrSet load_iset (LibraryManager &libManager) {

    libManager.scan(searchDir, dlExt);

    PluginManager& plugManager = PluginManager::getInstance();

    for(const auto& pluginInfo : plugManager.getPlugins()) {
        const InstructionSetPlugin& plugin = pluginInfo.second;
        std::cout << fmt::format("loaded plugin \"{}\":{}", 
        plugin.getName(), plugin.getVersion())  << std::endl;
    }

    InstrSet my_iset = InstrSet(); 

    for(const auto& pluginInfo : plugManager.getPlugins()) {
        const InstructionSetPlugin& plugin = pluginInfo.second;
        my_iset += plugin.getInstrSet();
    }

    return my_iset;
}