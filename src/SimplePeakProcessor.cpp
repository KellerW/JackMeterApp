#include "SimplePeakProcessor.hpp"
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>
#include <chrono>
#include <mutex>

namespace jackmeter {

SimplePeakProcessor::SimplePeakProcessor(std::string_view name, float threshold)
    : m_name(name), m_latestPeak(0.0f), 
      m_minPeak(std::numeric_limits<float>::infinity()),  
      m_maxPeak(-std::numeric_limits<float>::infinity()), 
      m_noiseThreshold(threshold), m_currentPPM(0.0f), m_peakHold(0.0f),
      m_decayRate(0.05f), m_peakHoldTime(2.0f), m_lastTime(std::chrono::steady_clock::now())   
{
    #ifdef DEBUG
        fmt::print("SimplePeakProcessor created with threshold: {}\n", threshold); 
    #endif
}

void SimplePeakProcessor::Process(float* samples, uint32_t nSamples)
{
    float localMax = *std::max_element(samples, samples + nSamples);

    m_latestPeak.store(localMax);
    m_maxPeak.store(std::max(m_maxPeak.load(), localMax));
    std::lock_guard<std::mutex> lock(m_mutex);

    auto currentTime = std::chrono::steady_clock::now();
    std::chrono::duration<float> elapsed = currentTime - m_lastTime;

    if (localMax > m_currentPPM) 
    {
        m_currentPPM = localMax;
        m_peakHold = localMax;
        m_lastTime = currentTime;
    } 
    else if (elapsed.count() > m_peakHoldTime) 
    {
        // Decay the peak hold if the elapsed time is greater than the hold time
        m_currentPPM -= m_decayRate * elapsed.count();
        if (m_currentPPM < 0.0f) {
            m_currentPPM = 0.0f;
        }
        m_lastTime = currentTime;
    }

    m_signalDetected.store(localMax >= m_noiseThreshold);
}


bool SimplePeakProcessor::SignalDetected() const
{
    return m_signalDetected.load();
}

float SimplePeakProcessor::GetCurrentPPMDb() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return 20.0f * log10f(m_currentPPM);
}

float SimplePeakProcessor::GetPeakHoldDb() const
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return 20.0f * log10f(m_peakHold);
}

void SimplePeakProcessor::ResetPeakHold()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_peakHold = 0.0f;
}

float SimplePeakProcessor::GetLatestPeak() const
{ 
    return m_latestPeak.load();
}

float SimplePeakProcessor::GetLatestPeakDb() const
{
    return 20.0f * log10f(m_latestPeak.load());
}

float SimplePeakProcessor::GetMinPeak() const
{
    return m_minPeak.load();
}

float SimplePeakProcessor::GetMinPeakDb() const
{
    float minPeak = m_minPeak.load();  
    if (minPeak == 0.0f) {
        return -std::numeric_limits<float>::infinity();  
    }
    return 20.0f * log10f(minPeak);  
}

float SimplePeakProcessor::GetMaxPeak() const
{
    return m_maxPeak.load();
}

float SimplePeakProcessor::GetMaxPeakDb() const
{
    float maxPeak = m_maxPeak.load();
    if (maxPeak == 0.0f) {
        return -std::numeric_limits<float>::infinity();
    }
    return 20.0f * log10f(maxPeak);
}

std::string_view SimplePeakProcessor::GetName() const
{
    return m_name;
}

}  // namespace jackmeter
