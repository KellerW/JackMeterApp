#include "FFTProcessor.hpp"
#include <cmath>
#include <iostream>

namespace jackmeter
{

FFTProcessor::FFTProcessor(size_t bufferSize)
    : bufferSize(bufferSize), fftInput(nullptr), fftOutput(nullptr), fftPlan(nullptr),
      m_name("FFTProcessor"), signalDetected(false)
{

    fftInput  = (double *)fftw_malloc(sizeof(double) * bufferSize);
    fftOutput = (fftw_complex *)fftw_malloc(sizeof(fftw_complex) * bufferSize);

    fftPlan = fftw_plan_dft_r2c_1d(bufferSize, fftInput, fftOutput, FFTW_ESTIMATE);

    frequencyBins.resize(bufferSize / 2);
}

FFTProcessor::~FFTProcessor()
{
    fftw_destroy_plan(fftPlan);
    fftw_free(fftInput);
    fftw_free(fftOutput);
}

void FFTProcessor::Process(float *samples, uint32_t nSamples)
{
    std::lock_guard<std::mutex> lock(m_mutex);

    for (uint32_t i = 0; i < nSamples && i < bufferSize; ++i)
    {
        fftInput[i] = static_cast<double>(samples[i]);
    }

    fftw_execute(fftPlan);

    signalDetected = false;
    for (size_t i = 0; i < bufferSize / 2; ++i)
    {
        double real      = fftOutput[i][0];
        double imag      = fftOutput[i][1];
        double magnitude = sqrt(real * real + imag * imag);
        frequencyBins[i] = magnitude;

        // Example logic: Signal detection threshold
        if (magnitude > 0.01)
        {
            signalDetected = true; // Set true if any magnitude exceeds the threshold
        }
    }
}

bool FFTProcessor::SignalDetected() const
{
    return signalDetected; // Return whether a signal was detected
}

std::string_view FFTProcessor::GetName() const { return m_name; }

const std::vector<double> &FFTProcessor::GetFrequencyBins() const { return frequencyBins; }

} // namespace jackmeter
