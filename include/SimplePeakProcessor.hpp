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

    double GetLatestPeak() const;
    double GetLatestPeakDb() const;

    double GetMinPeak() const;
    double GetMinPeakDb() const;

    double GetMaxPeak() const;
    double GetMaxPeakDb() const;

    std::string_view GetName() const override;

private:
    std::string m_name;
    std::atomic<float> m_latestPeak;
    float m_minPeak=0;
    std::atomic<float> m_maxPeak;
    std::atomic<float> m_noiseThreshold;
   
    std::chrono::time_point<std::chrono::steady_clock> m_lastTime;
    mutable std::mutex m_mutex;
    std::atomic<bool> m_signalDetected{false};
};

}  // namespace jackmeter
