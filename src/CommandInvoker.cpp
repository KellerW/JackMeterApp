#include "CommandInvoker.hpp"

namespace jackmeter
{
namespace invoker
{

// Add a command to the invoker's queue
void CommandInvoker::addCommand(std::shared_ptr<jackmeter::commands::Command> command)
{
    commandQueue.push_back(command);
}

// Execute all commands in the queue and clear it afterward
void CommandInvoker::executeCommands()
{
    for (const auto &command : commandQueue)
    {
        command->execute();
    }
    commandQueue.clear(); // Clear the queue after execution
}

} // namespace invoker
} // namespace jackmeter
