
#include "gen.h"

#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>
#include "../plugin/plugin_internals/api/include/plugin_manager.h"


constexpr auto dlExt = ".so";


InstrSet load_iset (LibraryManager &libManager) {

    const char *searchDir = std::getenv("PLUGINS_PATH");
    std::cout << "Loaded from:" << searchDir << std::endl;
    if (!searchDir) throw std::runtime_error ("Bad environmental variable\n");

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