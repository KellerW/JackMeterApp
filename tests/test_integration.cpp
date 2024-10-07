#include "CommandInvoker.h"
#include "Commands.hpp"  // Updated to match your project setup
#include <getopt.h>
#include <gtest/gtest.h>
#include <memory>

TEST(IntegrationTest, FullCommandFlow)
{
    // Mocking command line arguments
    int argc     = 5;
    char *argv[] = {"./app", "-f", "10", "-d", "20"};

    // Initial values for variables
    int rate            = 8;  // Initial value for rate
    int duration        = 0;  // Initial value for duration
    int max_count       = 0;
    bool connected_only = false;
    bool plain_output   = false;
    bool no_matching    = false;

    // Initialize the CommandInvoker
    jackmeter::invoker::CommandInvoker invoker;

    // Reset getopt's state (especially important when testing with multiple test cases)
    optind = 1; // Reset to the beginning
    int opt;
    
    // Parse command line options as provided by `argv`
    while ((opt = getopt(argc, argv, "f:d:n:pcxh")) != -1)
    {
        switch (opt)
        {
            case 'f':
                // Adding FrequencyCommand to the invoker
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::FrequencyCommand>(rate, optarg));
                break;
            case 'd':
                // Adding DurationCommand to the invoker
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::DurationCommand>(duration, optarg));
                break;
            case 'n':
                // Adding MaxCountCommand to the invoker
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::MaxCountCommand>(max_count, optarg));
                break;
            case 'c':
                // Adding ConnectedOnlyCommand to the invoker
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::ConnectedOnlyCommand>(connected_only));
                break;
            case 'p':
                // Adding PlainOutputCommand to the invoker
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::PlainOutputCommand>(plain_output));
                break;
            case 'x':
                // Adding NoMatchingCommand to the invoker
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::NoMatchingCommand>(no_matching));
                break;
            case 'h':
                // Adding HelpCommand to the invoker
                invoker.addCommand(std::make_shared<jackmeter::commands::HelpCommand>());
                break;
            default:
                // Adding UsageCommand if the option is unrecognized
                invoker.addCommand(std::make_shared<jackmeter::commands::UsageCommand>());
        }
    }

    // Execute all commands added to the invoker
    invoker.executeCommands();

    // Validate if the rate and duration have been updated as expected
    ASSERT_EQ(rate, 10);      // Expected rate to be 10
    ASSERT_EQ(duration, 20);  // Expected duration to be 20
}
