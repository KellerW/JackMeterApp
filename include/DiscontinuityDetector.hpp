#pragma once

#include "IProcessor.hpp"
#include "wavelib.h"
#include "wauxlib.h"
#include <vector>
#include <string>
#include <cmath>

namespace jackmeter
{
class DiscontinuityProcessor : public jackmeter::IProcessor {
public:
    DiscontinuityProcessor(int signal_length);
    virtual ~DiscontinuityProcessor();

    void Process(float* samples, uint32_t nSamples) override;
    std::string_view GetName() const override;
    bool SignalDetected() const override;

    // Method to return pre-computed discontinuities
    std::vector<int> getDiscontinuityIndices() const;

private:
    std::string name = "DiscontinuityProcessor";
    uint32_t signal_length;
    bool discontinuity_detected = false;

    // Vector to store detected discontinuity indices
    std::vector<int> discontinuityIndices;  // Add this declaration

    // Wavelet transformation objects
    wave_object wave;
    wt_object wt;
    std::vector<double> wavelet_output;
};

} // namespace jackmeter
