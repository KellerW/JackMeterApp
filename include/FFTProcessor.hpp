#pragma once

#include "IProcessor.hpp"
#include <fftw3.h>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

namespace jackmeter
{

class FFTProcessor : public IProcessor
{
  public:
    FFTProcessor(size_t bufferSize);
    ~FFTProcessor();

    void Process(float *samples, uint32_t nSamples) override;
    bool SignalDetected() const override; // Implementing the pure virtual function
    std::string_view GetName() const override;

    const std::vector<double> &GetFrequencyBins() const;

  private:
    size_t bufferSize;
    double *fftInput;
    fftw_complex *fftOutput;
    fftw_plan fftPlan;

    std::vector<double> frequencyBins;

    std::string m_name;
    mutable std::mutex m_mutex;
    bool signalDetected; // To keep track if a signal is detected
};

} // namespace jackmeter
