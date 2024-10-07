#include <atomic>
#include <chrono>
#include <cmath>   // For std::abs
#include <cstring> // For memset
#include <iostream>
#include <jack/jack.h>
#include <sndfile.h>
#include <thread>

jack_client_t *client;
jack_port_t *output_port_left;
jack_port_t *output_port_right;

SNDFILE *sndfile;
SF_INFO sfinfo;

float *audio_buffer      = nullptr;
sf_count_t total_frames  = 0;
sf_count_t current_frame = 0;

std::atomic<float> peak_left(0.0f);
std::atomic<float> peak_right(0.0f);
std::atomic<bool> discontinuity_detected(false);

int process(jack_nframes_t nframes, void *arg)
{
    float *out_left  = (float *)jack_port_get_buffer(output_port_left, nframes);
    float *out_right = (float *)jack_port_get_buffer(output_port_right, nframes);

    // Calculate remaining frames
    sf_count_t frames_remaining = total_frames - current_frame;
    sf_count_t frames_to_copy   = nframes;

    if (frames_remaining < nframes)
    {
        frames_to_copy = frames_remaining;

        // Set discontinuity flag if we can't provide enough data
        discontinuity_detected.store(true);
    }

    // Copy audio data to JACK output buffers
    if (frames_to_copy > 0)
    {
        for (unsigned int i = 0; i < frames_to_copy; ++i)
        {
            // Get sample indices
            sf_count_t idx = (current_frame + i) * sfinfo.channels;

            // Fetch samples
            float sample_left  = audio_buffer[idx];
            float sample_right = sfinfo.channels > 1 ? audio_buffer[idx + 1] : sample_left;

            // Write to output buffers
            out_left[i]  = sample_left;
            out_right[i] = sample_right;

            // Peak detection
            float abs_left  = std::abs(sample_left);
            float abs_right = std::abs(sample_right);

            // Update peak values atomically
            float current_peak_left = peak_left.load();
            if (abs_left > current_peak_left)
            {
                peak_left.store(abs_left);
            }

            float current_peak_right = peak_right.load();
            if (abs_right > current_peak_right)
            {
                peak_right.store(abs_right);
            }
        }
        current_frame += frames_to_copy;
    }

    // Zero the remaining buffer if at the end
    if (frames_to_copy < nframes)
    {
        memset(&out_left[frames_to_copy], 0, (nframes - frames_to_copy) * sizeof(float));
        memset(&out_right[frames_to_copy], 0, (nframes - frames_to_copy) * sizeof(float));

        // Optionally loop the audio
        // current_frame = 0;
    }

    return 0;
}

int init_jack(const char *client_name)
{
    jack_status_t status;
    client = jack_client_open(client_name, JackNullOption, &status);
    if (client == nullptr)
    {
        std::cerr << "Failed to connect to JACK server\n";
        return 1;
    }

    // Set process callback
    jack_set_process_callback(client, process, nullptr);

    // Register output ports
    output_port_left =
        jack_port_register(client, "output_left", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
    output_port_right =
        jack_port_register(client, "output_right", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);

    if (output_port_left == nullptr || output_port_right == nullptr)
    {
        std::cerr << "Failed to register JACK ports\n";
        return 1;
    }

    if (jack_activate(client))
    {
        std::cerr << "Cannot activate client";
        return 1;
    }

    return 0;
}

int load_wav(const char *filename)
{
    memset(&sfinfo, 0, sizeof(sfinfo));

    sndfile = sf_open(filename, SFM_READ, &sfinfo);
    if (!sndfile)
    {
        std::cerr << "Failed to open audio file: " << filename << "\n";
        return 1;
    }

    total_frames = sfinfo.frames;
    audio_buffer = new float[total_frames * sfinfo.channels];

    sf_count_t readcount = sf_readf_float(sndfile, audio_buffer, total_frames);
    if (readcount != total_frames)
    {
        std::cerr << "Failed to read audio data from file\n";
        return 1;
    }

    sf_close(sndfile);
    return 0;
}

void connect_ports()
{
    const char **ports =
        jack_get_ports(client, nullptr, nullptr, JackPortIsPhysical | JackPortIsInput);
    if (ports == nullptr)
    {
        std::cerr << "No physical playback ports\n";
        return;
    }

    if (jack_connect(client, jack_port_name(output_port_left), ports[0]))
    {
        std::cerr << "Cannot connect left channel\n";
    }

    if (jack_connect(client, jack_port_name(output_port_right), ports[1]))
    {
        std::cerr << "Cannot connect right channel\n";
    }

    jack_free(ports);
}

int main(int argc, char *argv[])
{
    // if (argc < 2) {
    //     std::cerr << "Usage: ./jack_play_wav filename.wav\n";
    //     return 1;
    // }

    // if (load_wav(argv[1])) {
    //     return 1;
    // }

    // if (init_jack("WAV_Player")) {
    //     return 1;
    // }

    // connect_ports();

    // std::cout << "Playing audio... Press Ctrl+C to stop.\n";

    // // Keep the program running
    // while (current_frame < total_frames) {
    //     // Read and reset peak values
    //     float peak_l = peak_left.exchange(0.0f);
    //     float peak_r = peak_right.exchange(0.0f);

    //     // Check for discontinuity
    //     bool discontinuity = discontinuity_detected.exchange(false);

    //     // Display peak values and discontinuity status
    //     std::cout << "Peak Left: " << peak_l << "  Peak Right: " << peak_r;
    //     if (discontinuity) {
    //         std::cout << "  [Discontinuity Detected]";
    //     }
    //     std::cout << "\r" << std::flush;

    //     // Sleep for a short duration
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100));
    // }

    // // Clean up
    // jack_client_close(client);
    // delete[] audio_buffer;

    // return 0;
}
