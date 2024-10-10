#pragma once
#include <vector>
#include <memory>
#include "Commands.hpp"

namespace jackmeter {
namespace invoker {

class CommandInvoker {
private:
    std::vector<std::shared_ptr<jackmeter::commands::Command>> commandQueue;

public:
    // Add a command to the invoker's queue
    void addCommand(std::shared_ptr<jackmeter::commands::Command> command);

    // Execute all commands in the queue and clear it afterward
    void executeCommands();
};

}  // namespace invoker
}  // namespace jackmeter
