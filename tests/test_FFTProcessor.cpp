#include "FFTProcessor.hpp"
#include <gtest/gtest.h>
#include <vector>
#include <cmath>

using namespace jackmeter;

// Fixture for setting up an FFTProcessor instance before running each test
class FFTProcessorTest : public ::testing::Test {
protected:
    void SetUp() override {
        fftProcessor = std::make_unique<FFTProcessor>(bufferSize);
    }

    // Buffer size used for the tests
    size_t bufferSize = 1024;

    // The FFTProcessor instance
    std::unique_ptr<FFTProcessor> fftProcessor;
};

// Test the constructor
TEST_F(FFTProcessorTest, ConstructorInitializesCorrectly) {
    ASSERT_EQ(fftProcessor->GetFrequencyBins().size(), bufferSize / 2);
    EXPECT_EQ(fftProcessor->SignalDetected(), false);
}

// Test processing a simple sinusoidal signal
TEST_F(FFTProcessorTest, ProcessSinusoidalSignal) {
    std::vector<float> samples(bufferSize);
    double frequency = 100.0;
    double sampleRate = 1000.0;

    // Generate a sinusoidal signal with the given frequency
    for (size_t i = 0; i < bufferSize; ++i) {
        samples[i] = static_cast<float>(sin(2 * M_PI * frequency * i / sampleRate));
    }

    // Process the sinusoidal signal
    fftProcessor->Process(samples.data(), samples.size());

    // Get the frequency bins and check if signal detection was successful
    const auto &bins = fftProcessor->GetFrequencyBins();
    EXPECT_TRUE(fftProcessor->SignalDetected());

    // Check if the FFT contains a significant peak at the expected frequency
    double maxMagnitude = 0.0;
    size_t maxIndex = 0;
    for (size_t i = 0; i < bins.size(); ++i) {
        if (bins[i] > maxMagnitude) {
            maxMagnitude = bins[i];
            maxIndex = i;
        }
    }

    // Calculate the frequency from the index
    double detectedFrequency = maxIndex * sampleRate / bufferSize;

    // Allow some tolerance for frequency detection
    EXPECT_NEAR(detectedFrequency, frequency, 10.0);
}

// Test processing a zero signal (no input)
TEST_F(FFTProcessorTest, ProcessZeroSignal) {
    std::vector<float> samples(bufferSize, 0.0f);

    // Process the zero signal
    fftProcessor->Process(samples.data(), samples.size());

    // Check that no signal was detected
    EXPECT_FALSE(fftProcessor->SignalDetected());

    // Check that all frequency bins are zero
    const auto &bins = fftProcessor->GetFrequencyBins();
    for (double magnitude : bins) {
        EXPECT_EQ(magnitude, 0.0);
    }
}

// Test that the GetName method returns the correct name
TEST_F(FFTProcessorTest, GetName) {
    EXPECT_EQ(fftProcessor->GetName(), "FFTProcessor");
}

// Test a random signal to verify signal detection logic
TEST_F(FFTProcessorTest, ProcessRandomSignal) {
    std::vector<float> samples(bufferSize);

    // Generate random values for the samples
    for (size_t i = 0; i < bufferSize; ++i) {
        samples[i] = static_cast<float>(rand() % 1000) / 1000.0f;
    }

    // Process the random signal
    fftProcessor->Process(samples.data(), samples.size());

    // Check if any signal was detected (based on the threshold logic)
    EXPECT_TRUE(fftProcessor->SignalDetected());
}
