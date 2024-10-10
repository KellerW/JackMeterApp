#include <gtest/gtest.h>
#include <memory>
#include "SimplePeakProcessor.hpp"
#include <cmath>
#include <vector>
#include <thread>

using namespace jackmeter;

// Test fixture for SimplePeakProcessor
class SimplePeakProcessorTest : public ::testing::Test {
protected:
    std::unique_ptr<SimplePeakProcessor> processor;

    void SetUp() override {
        // Initialize SimplePeakProcessor with a name and noise threshold of 0.01
        processor = std::make_unique<SimplePeakProcessor>("TestProcessor", 0.01f);
    }
};

constexpr int noise_th = 0.001f;


/// Test case to verify signal detection
TEST(SimplePeakProcessorTest, SignalDetection)
{
    jackmeter::SimplePeakProcessor processor("testProcessor", noise_th);

    std::vector<float> silentData = { 0.0f, 0.0f, 0.0f, 0.0f };
    processor.Process(silentData.data(), silentData.size());
    EXPECT_FALSE(processor.SignalDetected());  // No signal detected

    std::vector<float> nonSilentData = { 0.1f, -0.2f, 0.0f, 0.5f };
    processor.Process(nonSilentData.data(), nonSilentData.size());
    EXPECT_TRUE(processor.SignalDetected());  // Signal detected
}

// Test case to verify max peak values in dB
TEST(SimplePeakProcessorTest, MaxPeakDbValues)
{
    jackmeter::SimplePeakProcessor processor("testProcessor", noise_th);
    std::vector<float> data = { 0.5f, 0.8f, -0.7f, 1.0f };
    processor.Process(data.data(), data.size());
    float expectedMaxPeakDb = 20.0f * log10f(1.0f);
    EXPECT_FLOAT_EQ(processor.GetMaxPeakDb(), expectedMaxPeakDb);
}

// Test case to verify min peak values in dB
TEST(SimplePeakProcessorTest, MinPeakDbValues)
{
    jackmeter::SimplePeakProcessor processor ("testProcessor", noise_th);

    std::vector<float> data = { 0.5f, 0.8f, -0.7f, 1.0f };
    processor.Process(data.data(), data.size());

    float minPeakDb = processor.GetMinPeakDb();
    
    EXPECT_NEAR(minPeakDb, -3.0980396, 1e-6); 
}




