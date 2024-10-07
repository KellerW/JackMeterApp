#include <iostream>
#include <vector>
#include <sndfile.h>  // Include libsndfile header
#include "wavelib.h"
#include "wauxlib.h"

// Function to load a .wav file
bool load_wav_file(const std::string& filename, std::vector<float>& audio_data, int& sample_rate) {
    SF_INFO sf_info;
    SNDFILE* sndfile = sf_open(filename.c_str(), SFM_READ, &sf_info);

    if (!sndfile) {
        std::cerr << "Error opening .wav file: " << filename << std::endl;
        return false;
    }

    sample_rate = sf_info.samplerate;
    int num_samples = sf_info.frames * sf_info.channels;

    // Resize the vector to hold the audio samples
    audio_data.resize(num_samples);

    // Read the audio data (libsndfile automatically handles multi-channel files)
    sf_read_float(sndfile, audio_data.data(), num_samples);

    // Close the .wav file
    sf_close(sndfile);

    return true;
}

// Function to apply wavelet transform and detect discontinuities
void detect_discontinuities_wavelet(const std::vector<float>& audio_data) {
    int signal_length = audio_data.size();

    // Initialize the Daubechies wavelet (order 4)
    wave_object wave = wave_init("db4");

    // Convert the input audio data (float) to double precision
    std::vector<double> audio_data_double(audio_data.begin(), audio_data.end());

    // Initialize the wavelet transform (1 level of decomposition for simplicity)
    wt_object wt = wt_init(wave, "dwt", signal_length, 1);

    // Perform the wavelet transform (passing the double-precision array)
    dwt(wt, audio_data_double.data());

    // Analyze the wavelet coefficients for discontinuities
    std::cout << "Discontinuity Detection:\n";
    for (int i = 1; i < wt->outlength; ++i) {
        if (std::abs(wt->output[i] - wt->output[i - 1]) > 2.0) {  // Threshold for detection
            std::cout << "Discontinuity detected at index " << i << std::endl;
        }
    }

    // Free the memory used by wave and wt
    wave_free(wave);
    wt_free(wt);
}

int main() {
    std::vector<float> audio_data;
    int sample_rate = 0;

    // Load the .wav file (replace with the actual path to your .wav file)
    std::string filename = "/home/wagnerkeller/projects/jackmeter/discontinuity_signal.wav";

    if (load_wav_file(filename, audio_data, sample_rate)) {
        std::cout << "Loaded .wav file with sample rate: " << sample_rate << "\n";
        std::cout << "Total samples: " << audio_data.size() << "\n";

        // Detect discontinuities using wavelet transform
        detect_discontinuities_wavelet(audio_data);
    } else {
        std::cerr << "Failed to load .wav file\n";
    }

    return 0;
}
