#include "SimplePeakProcessor.hpp"
#include <cmath>
#include <limits>
#include <algorithm>
#include <string>
#include <chrono>
#include <mutex>
#include <fmt/core.h>
#include <fmt/ostream.h>

namespace jackmeter {

SimplePeakProcessor::SimplePeakProcessor(std::string_view name, float threshold)
    : m_name(name), m_latestPeak(0.0f), 
      m_minPeak(std::numeric_limits<double>::infinity()),  
      m_maxPeak(-std::numeric_limits<double>::infinity()), 
      m_noiseThreshold(threshold), m_lastTime(std::chrono::steady_clock::now())   
{
    
}

void SimplePeakProcessor::Process(float* samples, uint32_t nSamples)
{
    double bufferMaxPeak = -std::numeric_limits<double>::infinity();
    double bufferMinPeak = std::numeric_limits<double>::infinity();

    
    for (uint32_t i = 0; i < nSamples; i++) 
    {
        if (samples[i] > bufferMaxPeak) 
        {
            bufferMaxPeak = samples[i];  
        }
        if (samples[i] < bufferMinPeak) 
        {
            bufferMinPeak = samples[i];  
        }
    }
   
    
    m_latestPeak.store(std::max(fabs(bufferMaxPeak), fabs(bufferMinPeak)));
   
    
    if (bufferMaxPeak > m_maxPeak.load())
    {
        
        m_maxPeak.store(bufferMaxPeak);
        if (!m_signalDetected && (fabs(bufferMaxPeak) > m_noiseThreshold)) 
        {
            m_minPeak = bufferMinPeak;
            m_signalDetected.store(true);
        }
    }
        
    if (bufferMinPeak < m_minPeak) 
    {
        m_minPeak= bufferMinPeak;
    }
}

bool SimplePeakProcessor::SignalDetected() const
{
    return m_signalDetected.load();
}

double SimplePeakProcessor::GetLatestPeak() const
{ 
    float latestPeak = m_latestPeak.load();
    return latestPeak;
}

double SimplePeakProcessor::GetLatestPeakDb() const
{
    float dbValue = 20.0f * log10f(m_latestPeak.load());
    return dbValue;
}

double SimplePeakProcessor::GetMinPeak() const
{
    double minPeak = m_minPeak;
   
    return minPeak;
   
}

double SimplePeakProcessor::GetMinPeakDb() const
{
    double dbValue = 0;
    
    if (this->m_minPeak == 0.0f) 
    {
       return -std::numeric_limits<float>::infinity();  
    }

    if (this->m_minPeak < 0.0f) 
    {
        dbValue = 20.0f * log10f(fabs(this->m_minPeak));
    }
    else
    {
        dbValue = 20.0f * log10f(this->m_minPeak);
    }
    
    return dbValue;
}

double SimplePeakProcessor::GetMaxPeak() const
{
    double maxPeak = m_maxPeak.load();
    return maxPeak;
}

double SimplePeakProcessor::GetMaxPeakDb() const
{
    double maxPeak = m_maxPeak.load();
    if (maxPeak == 0.0f) {
        return -std::numeric_limits<double>::infinity();
    }
    double dbValue = 20.0f * log10f(maxPeak);
    return dbValue;
}

std::string_view SimplePeakProcessor::GetName() const
{
    return m_name;
}

}  // namespace jackmeter
