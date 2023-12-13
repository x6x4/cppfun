#include <string>
#include <chrono>
#include "fmt/chrono.h"

#include "../api/include/plugin.h"
#include "../api/include/plugin_manager.h"

class CurrentTimePlugin: public OperatorPlugin {
public:

  std::string getName() const override {
    return "current_time_plugin";
  }

  std::string getVersion() const override {
    return "1.0.0";
  }

  std::string getMsg() const override {
    return fmt::format("Current time is {:%T}", std::chrono::system_clock::now());
  }
};

static CurrentTimePlugin plugin;
static auto token = PluginToken(plugin);
