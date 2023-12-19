
#include "gen.h"

#include <filesystem>
#include <string>
#include <vector>
#include "../plugin/plugin_internals/api/include/plugin_manager.h"
#include "../plugin/plugin_internals/library_manager/include/library_manager.h"


constexpr auto dlExt = ".so";
constexpr auto searchDir = "/home/cracky/cppfun/3/build/plugin/plugins";

InstrSet *load_iset () {
    LibraryManager libManager;
    libManager.scan(searchDir, dlExt);

    PluginManager& plugManager = PluginManager::getInstance();

    for(const auto& pluginInfo : plugManager.getPlugins()) {
        const InstructionSetPlugin& plugin = pluginInfo.second;
        std::cout << fmt::format("loaded plugin \"{}\":{}", 
        plugin.getName(), plugin.getVersion())  << std::endl;
    }

    InstrSet *my_iset = new InstrSet(); 

    for(const auto& pluginInfo : plugManager.getPlugins()) {
        const InstructionSetPlugin& plugin = pluginInfo.second;
        *my_iset += plugin.getInstrSet();
    }

    Operator test = my_iset->FindOper("inc");
    my_std::Vec<std::unique_ptr<Operand>> b;
    b.push_back({std::make_unique<Operand>(Operand())});
    test.oper(b);

    return my_iset;
}