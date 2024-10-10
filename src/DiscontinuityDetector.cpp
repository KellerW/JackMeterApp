#include "DiscontinuityDetector.hpp"
#include <iostream>

namespace jackmeter
{
DiscontinuityProcessor::DiscontinuityProcessor(int signal_length)
    : signal_length(signal_length), discontinuity_detected(false) {
    // Initialize the Daubechies wavelet (order 4)
    wave = wave_init("db4");

    // Initialize the wavelet transform (1 level of decomposition for simplicity)
    wt = wt_init(wave, "dwt", signal_length, 1);

    // Initialize the wavelet output buffer
    wavelet_output.resize(signal_length);
}

DiscontinuityProcessor::~DiscontinuityProcessor() 
{
    wave_free(wave);
    wt_free(wt);
}

void DiscontinuityProcessor::Process(float* samples, uint32_t nSamples) {
    // Clear previous results
    discontinuityIndices.clear();  // Clear the discontinuity indices

    if (nSamples != signal_length) {
        std::cerr << "Error: signal length does not match processor initialization. Expected: "
                  << signal_length << ", Got: " << nSamples << std::endl;
        return;
    }

    // Convert input samples to double precision
    std::vector<double> samples_double(samples, samples + nSamples);
   
    // Perform the wavelet transform
    dwt(wt, samples_double.data());

    // Analyze the wavelet coefficients for discontinuities
    discontinuity_detected = false;

    for (int i = 1; i < wt->outlength; ++i) {
        if (std::abs(wt->output[i] - wt->output[i - 1]) > 2) {  // Threshold for detection
            discontinuity_detected = true;
            discontinuityIndices.push_back(i); // Store the detected index
        }
    }

    // Fix the comparison by casting wt->outlength to std::size_t
    if (wavelet_output.size() != static_cast<std::size_t>(wt->outlength)) {
        wavelet_output.resize(wt->outlength);  // Resize to match the output length
    }

    // Copy the wavelet output for further analysis if needed
    for (int i = 0; i < wt->outlength; ++i) {
        wavelet_output[i] = wt->output[i];
    }
}



std::string_view DiscontinuityProcessor::GetName() const {
    return name;
}

bool DiscontinuityProcessor::SignalDetected() const {
    return discontinuity_detected;
}

// Return the detected discontinuities
std::vector<int> DiscontinuityProcessor::getDiscontinuityIndices() const {
    return discontinuityIndices;  // Return the discontinuity indices
}

} // namespace jackmeter
