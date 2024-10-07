#pragma once

#include "IProcessor.hpp"
#include <atomic>
#include <string>
#include <mutex>
#include <cstdint>
#include <chrono>

namespace jackmeter {

class SimplePeakProcessor : public IProcessor {
public:
    SimplePeakProcessor(std::string_view name, float threshold);

    void Process(float* samples, uint32_t nSamples) override;
    bool SignalDetected() const override;

    float GetCurrentPPMDb() const override;
    float GetPeakHoldDb() const override;
    void ResetPeakHold() override;

    float GetLatestPeak() const;
    float GetLatestPeakDb() const;

    float GetMinPeak() const;
    float GetMinPeakDb() const;

    float GetMaxPeak() const;
    float GetMaxPeakDb() const;

    std::string_view GetName() const override;

private:
    std::string m_name;
    std::atomic<float> m_latestPeak;
    std::atomic<float> m_minPeak;
    std::atomic<float> m_maxPeak;
    std::atomic<float> m_noiseThreshold;
    float m_currentPPM;
    float m_peakHold;
    float m_decayRate;
    float m_peakHoldTime;

    std::chrono::time_point<std::chrono::steady_clock> m_lastTime;
    mutable std::mutex m_mutex;
    std::atomic<bool> m_signalDetected{false};
};

}  // namespace jackmeter
