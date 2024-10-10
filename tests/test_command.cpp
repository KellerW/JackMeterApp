#include "CommandInvoker.hpp"
#include "Commands.hpp"
#include "JackClient.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

// Mock the JackClient to test commands that depend on it
class MockJackClient : public jackmeter::JackClient
{
  public:
    MockJackClient(Config &mockConfig) : JackClient("test", mockConfig) {}

    MOCK_METHOD(void, Activate, (), (override));
    MOCK_METHOD(void, Deactivate, (), (override));
    MOCK_METHOD(void, Connect, (), (override));
};
// Unit test for CommandInvoker
TEST(CommandInvokerTest, ExecuteCommands)
{
    jackmeter::invoker::CommandInvoker invoker;
    Config config; // Create a Config object

    invoker.addCommand(std::make_shared<jackmeter::commands::FrequencyCommand>(config, "10"));

    // Execute commands
    invoker.executeCommands();

    // Check if rate is set correctly
    ASSERT_EQ(config.rate, 10);
}

// Unit test for FrequencyCommand
TEST(FrequencyCommandTest, FrequencyChange)
{
    Config config; // Create a Config object
    jackmeter::commands::FrequencyCommand freqCommand(config, "10");

    freqCommand.execute();
    ASSERT_EQ(config.rate, 10); // Ensure rate is correctly set to 10
}

// Unit test for DurationCommand
TEST(DurationCommandTest, DurationChange)
{
    Config config; // Create a Config object
    jackmeter::commands::DurationCommand durationCommand(config, "20");

    durationCommand.execute();
    ASSERT_EQ(config.duration, 20); // Ensure duration is correctly set to 20
}

