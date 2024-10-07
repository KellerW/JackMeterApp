#pragma once
#include <string_view>
#include <cstdint>  // For uint32_t

namespace jackmeter {

class IProcessor {
public:
    virtual ~IProcessor() = default;

    // Declare the methods that subclasses must implement
    virtual void Process(float* samples, uint32_t nSamples) = 0;  // Ensure this matches exactly
    virtual bool SignalDetected() const = 0;

    virtual float GetCurrentPPMDb() const = 0;   // Ensure const-correctness
    virtual float GetPeakHoldDb() const = 0;
    virtual void ResetPeakHold() = 0;

    virtual std::string_view GetName() const = 0;
};

}  // namespace jackmeter
