#include <gtest/gtest.h>
#include "DiscontinuityDetector.hpp"
#include <vector>
#include <memory>

class DiscontinuityProcessorTest : public ::testing::Test {
protected:
    std::unique_ptr<jackmeter::DiscontinuityProcessor> discontinuityProcessor;
    int signal_length = 1024;  // Example signal length

    void SetUp() override {
        // Initialize the DiscontinuityProcessor with signal length
        discontinuityProcessor = std::make_unique<jackmeter::DiscontinuityProcessor>(signal_length);
    }
};

TEST_F(DiscontinuityProcessorTest, GetNameTest) {
    EXPECT_EQ(discontinuityProcessor->GetName(), "DiscontinuityProcessor");
}

TEST_F(DiscontinuityProcessorTest, NoDiscontinuityTest) {
    std::vector<float> smooth_signal(signal_length, 0.0f);  // Simulated smooth signal

    // Process the smooth signal
    discontinuityProcessor->Process(smooth_signal.data(), smooth_signal.size());

    // Ensure no discontinuities were detected
    EXPECT_FALSE(discontinuityProcessor->SignalDetected());
}

TEST_F(DiscontinuityProcessorTest, DiscontinuityDetectedTest) {
    std::vector<float> discontinuous_signal(signal_length, 0.0f);  // Simulated signal
    discontinuous_signal[500] = 10.0f;  // Introduce a discontinuity

    // Process the signal with a discontinuity
    discontinuityProcessor->Process(discontinuous_signal.data(), discontinuous_signal.size());

    // Ensure a discontinuity was detected
    EXPECT_TRUE(discontinuityProcessor->SignalDetected());
}

TEST_F(DiscontinuityProcessorTest, SignalLengthMismatchTest) {
    std::vector<float> wrong_length_signal(signal_length / 2, 0.0f);  // Signal with the wrong length

    // Expect that processing a signal of the wrong length triggers a length mismatch error
    testing::internal::CaptureStderr();
    discontinuityProcessor->Process(wrong_length_signal.data(), wrong_length_signal.size());
    std::string output = testing::internal::GetCapturedStderr();

    EXPECT_EQ(output, "Error: signal length does not match processor initialization. Expected: 1024, Got: 512\n");
    EXPECT_FALSE(discontinuityProcessor->SignalDetected());
}
