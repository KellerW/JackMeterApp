#include "DiscontinuityDetector.hpp"
#include <cmath>
#include <fmt/core.h>
#include <vector>
#include <algorithm>  // For std::min

namespace jackmeter {

DiscontinuityDetector::DiscontinuityDetector(float threshold) : threshold(threshold) {}

DiscontinuityDetector::~DiscontinuityDetector() {
    // No special resource cleanup needed for energy-based detection
}

void DiscontinuityDetector::Process(float* samples, uint32_t nSamples) {
    fmt::print("Processing {} samples for energy-based discontinuity detection.\n", nSamples);

    if (detectEnergy(samples, nSamples)) {
        fmt::print("Discontinuity detected in signal.\n");
    } else {
        fmt::print("No discontinuity detected in signal.\n");
    }
}

bool DiscontinuityDetector::detectEnergy(const float* samples, uint32_t nSamples) {
    // Choose a smaller frame size to ensure enough frames for detection
    const uint32_t frameSize = 256;  // Reduced frame size for smaller signals
    float prevEnergy = computeEnergy(samples, std::min(frameSize, nSamples));

    // Iterate through the signal in frames
    for (uint32_t i = frameSize; i < nSamples; i += frameSize) {
        uint32_t currentFrameSize = std::min(frameSize, nSamples - i);
        float currEnergy = computeEnergy(&samples[i], currentFrameSize);

        // Compute the difference between consecutive frames
        float energyDiff = std::abs(currEnergy - prevEnergy);

        fmt::print("Frame [{}-{}]: Prev Energy = {}, Curr Energy = {}, Diff = {}\n", i - frameSize, i, prevEnergy, currEnergy, energyDiff);

        // If the difference exceeds the threshold, it's a discontinuity
        if (energyDiff > threshold) {
            fmt::print("Discontinuity detected between frames {} and {} (Energy diff = {})\n", i - frameSize, i, energyDiff);
            return true;
        }

        prevEnergy = currEnergy;  // Update the previous energy for the next comparison
    }
    return false;
}

float DiscontinuityDetector::computeEnergy(const float* samples, uint32_t nSamples) {
    float energy = 0.0f;
    for (uint32_t i = 0; i < nSamples; ++i) {
        energy += samples[i] * samples[i];  // Sum of squared amplitudes
    }
    return energy / nSamples;  // Return average energy for normalization
}

}  // namespace jackmeter
