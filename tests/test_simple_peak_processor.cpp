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

// Test case to verify the signal processing
TEST_F(SimplePeakProcessorTest, ProcessSignalAboveThreshold) {
    // Create a signal where all samples are above the noise threshold
    std::vector<float> samples = {0.02f, 0.03f, 0.04f, 0.05f};
    processor->Process(samples.data(), samples.size());

    // Verify the latest peak
    EXPECT_NEAR(processor->GetLatestPeak(), 0.05f, 1e-6);
    // Verify signal detection
    EXPECT_TRUE(processor->SignalDetected());
}

// Test case to verify the signal processing below threshold
TEST_F(SimplePeakProcessorTest, ProcessSignalBelowThreshold) {
    // Create a signal where all samples are below the noise threshold
    std::vector<float> samples = {0.001f, 0.002f, 0.003f};
    processor->Process(samples.data(), samples.size());

    // Verify that the signal is not detected
    EXPECT_FALSE(processor->SignalDetected());
}

// Test case to verify the peak hold feature
TEST(SimplePeakProcessorTest, PeakHoldFeature)
{
    SimplePeakProcessor processor("testProcessor", 0.01f);
    std::vector<float> samples1 = {0.5f, 0.4f, 0.6f, 0.2f};

    processor.Process(samples1.data(), samples1.size());
    EXPECT_FLOAT_EQ(processor.GetPeakHoldDb(), 20.0f * log10f(0.6f));

    std::this_thread::sleep_for(std::chrono::seconds(3));  // Wait for decay to take effect

    processor.Process(samples1.data(), samples1.size());
    EXPECT_LT(processor.GetPeakHoldDb(), 20.0f * log10f(0.6f));  // Decayed
}

// Test case to verify the dB conversion of the current PPM
TEST_F(SimplePeakProcessorTest, CurrentPPMDbConversion) {
    // Process some samples
    std::vector<float> samples = {0.1f, 0.2f, 0.3f};
    processor->Process(samples.data(), samples.size());

    // Verify that the current PPM in dB is calculated correctly
    float currentPPM = 0.3f;
    float expectedPPMDb = 20.0f * std::log10(currentPPM);
    EXPECT_NEAR(processor->GetCurrentPPMDb(), expectedPPMDb, 1e-6);
}

// Test case to verify min and max peak values
TEST(SimplePeakProcessorTest, MinMaxPeakValues)
{
    SimplePeakProcessor processor("testProcessor", 0.01f);
    std::vector<float> samples = {0.5f, -0.3f, 0.9f, 0.1f, -0.8f};
    
    processor.Process(samples.data(), samples.size());
    
    EXPECT_FLOAT_EQ(processor.GetMinPeak(), -0.3f);
    EXPECT_FLOAT_EQ(processor.GetMaxPeak(), 0.9f);
}

