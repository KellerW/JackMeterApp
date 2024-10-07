#include <iostream>
#include <vector>
#include <cmath>
#include <sndfile.h>

// Function to generate a sine wave with discontinuities
std::vector<float> generate_sine_wave_with_discontinuities(int num_samples, int sample_rate) {
    std::vector<float> signal(num_samples);

    // Generate a sine wave and add discontinuities at specific indices
    for (int i = 0; i < num_samples; ++i) {
        // Generate a simple sine wave
        signal[i] = sin(2 * M_PI * i * 440.0 / sample_rate);  // 440 Hz sine wave

        // Introduce discontinuities at specific points
        if (i > num_samples / 4 && i < num_samples / 4 + 100) {
            signal[i] += 1.0f;  // Discontinuity 1: sudden increase in amplitude
        }
        if (i > num_samples / 2 && i < num_samples / 2 + 100) {
            signal[i] -= 1.5f;  // Discontinuity 2: sudden decrease in amplitude
        }
        if (i > 3 * num_samples / 4 && i < 3 * num_samples / 4 + 100) {
            signal[i] = 0.0f;   // Discontinuity 3: abrupt zeroing
        }
    }

    return signal;
}

// Function to write the signal to a .wav file
bool write_wav_file(const std::string& filename, const std::vector<float>& signal, int sample_rate) {
    SF_INFO sf_info;
    sf_info.samplerate = sample_rate;
    sf_info.frames = signal.size();
    sf_info.channels = 1;  // Mono
    sf_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;  // 16-bit PCM WAV format

    // Open the file for writing
    SNDFILE* sndfile = sf_open(filename.c_str(), SFM_WRITE, &sf_info);
    if (!sndfile) {
        std::cerr << "Error opening output file: " << filename << std::endl;
        return false;
    }

    // Write the signal to the .wav file
    sf_write_float(sndfile, signal.data(), signal.size());

    // Close the file
    sf_close(sndfile);

    return true;
}

int main() {
    int sample_rate = 44100;   // Standard sample rate for audio (44.1 kHz)
    int duration_seconds = 5;  // 5-second audio file
    int num_samples = sample_rate * duration_seconds;

    // Generate a sine wave with discontinuities
    std::vector<float> signal = generate_sine_wave_with_discontinuities(num_samples, sample_rate);

    // Write the signal to a .wav file
    std::string output_filename = "discontinuity_signal.wav";
    if (write_wav_file(output_filename, signal, sample_rate)) {
        std::cout << "WAV file generated: " << output_filename << std::endl;
    } else {
        std::cerr << "Failed to generate WAV file" << std::endl;
    }

    return 0;
}
