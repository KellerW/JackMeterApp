#include <gtest/gtest.h>
#include <memory>
#include "JackClient.hpp"
#include "Config.hpp"

class JackClientTest : public ::testing::Test {
protected:
    std::unique_ptr<jackmeter::JackClient> client;
    Config config;

    // Set up the test environment
    void SetUp() override {
        // Initialize a JackClient instance with a test name
        client = std::make_unique<jackmeter::JackClient>("TestClient", config);
    }

    // Clean up after the test
    void TearDown() override {
        client.reset();  // Ensure the client is destroyed after each test
    }
};

// Test case to check if the client is initialized properly
TEST_F(JackClientTest, InitializeClient) {
    ASSERT_NO_THROW({
        // Client should be initialized in SetUp
        ASSERT_NE(client, nullptr);
    });
}

// Test case to check if the client activates correctly
TEST_F(JackClientTest, ActivateClient) {
    ASSERT_NO_THROW({
        client->Activate();  // Client should activate without errors
    });
}

// Test case to check if playing a WAV file works
TEST_F(JackClientTest, PlayWavFile) {
    // Provide a path to a valid WAV file
    std::string wavFilePath = "/home/wagnerkeller/projects/jackmeter/wavfiles/test.wav";  // Make sure to use an actual file path

    // Attempt to play the WAV file
    ASSERT_NO_THROW({
        client->PlayWavFile(wavFilePath);
    });
}

// Test case to check if connecting ports works
TEST_F(JackClientTest, ConnectPorts) {
    ASSERT_NO_THROW({
        client->Connect();  // Client should connect ports without errors
    });
}


