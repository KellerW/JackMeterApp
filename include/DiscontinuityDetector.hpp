#pragma once

#include <vector>
#include "IProcessor.hpp"

namespace jackmeter {

class DiscontinuityDetector : public IProcessor {
public:
    DiscontinuityDetector(float threshold = 0.5f);  // Constructor with a default energy threshold
    virtual void Process(float* samples, uint32_t nSamples) final;  // Override the Process method
    virtual ~DiscontinuityDetector();  // Destructor

private:
    bool detectEnergy(const float* samples, uint32_t nSamples);  // Method to detect discontinuity based on energy
    float computeEnergy(const float* samples, uint32_t nSamples);  // Method to compute energy of a signal frame
    float threshold;  // Threshold for energy difference
};

}  // namespace jackmeter
