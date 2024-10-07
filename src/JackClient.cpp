#include "JackClient.hpp"
#include <fmt/core.h>  
#include <sndfile.h>   
#include <stdexcept>

namespace jackmeter {

JackClient::JackClient(std::string_view name, Config& config)
    : m_clientName(std::string(name)), m_client(nullptr), wavFile(nullptr), m_config(config), m_peakProcessor("teste", 0.01f)
    {

    jack_options_t options = JackNullOption;
    jack_status_t status;
    m_client = jack_client_open(m_clientName.c_str(), options, &status);

    if (!m_client)
        throw std::runtime_error(
            fmt::format("Could not create JACK client '{}', reason:{}", m_clientName, status));

    if (status & JackServerStarted)
        fmt::print(stderr, "jack_client_open started the JACK server, is there a bug?");

    if (status & JackNameNotUnique) {
        m_clientName = jack_get_client_name(m_client);
        fmt::print(stderr, "Client name not unique, assigned new name: '{}'.", m_clientName);
    }

    jack_set_process_callback(m_client, JackClient::OnJackProcess, this);

    m_bufSize = jack_get_buffer_size(m_client);
    m_sampleRate = jack_get_sample_rate(m_client);
}

JackClient::~JackClient() 
{
    for (auto pair : m_pairs) {
        auto srcPort = pair.first.first;
        auto sinkPort = pair.first.second;
        jack_disconnect(m_client, jack_port_name(srcPort), jack_port_name(sinkPort));
    }

    jack_deactivate(m_client);
    jack_set_process_callback(m_client, nullptr, nullptr);
    for (auto pair : m_pairs) 
    {
        auto sinkPort = pair.first.second;
        jack_port_unregister(m_client, sinkPort);
    }

    jack_client_close(m_client);

    if (wavFile) 
    {
        sf_close(wavFile);
        wavFile = nullptr;
    }
}

void JackClient::Activate() 
{
    auto res = jack_activate(m_client);
    if (res) 
    {
        throw std::runtime_error(fmt::format("Activating client '{}' failed with {}", m_clientName, res));
    }
}

void JackClient::Deactivate() { jack_deactivate(m_client); }

void JackClient::Connect() {
    for (auto pair : m_pairs) {
        auto srcPort  = pair.first.first;
        auto sinkPort = pair.first.second;
        if (jack_connect(m_client, jack_port_name(srcPort), jack_port_name(sinkPort))) {
            fmt::print(stderr, "Can't connect ports {} -> {}\n", jack_port_name(srcPort),
                       jack_port_name(sinkPort));
        } 
        else 
        {
            #ifdef DEBUG
                fmt::print("Connected ports {} -> {}\n", jack_port_name(srcPort), jack_port_name(sinkPort));
            #endif
        }
    }
}

void JackClient::AddProbe(std::string_view peer, std::shared_ptr<IProcessor> processor) {
    auto peerPort = jack_port_by_name(m_client, std::string(peer).c_str());
    if (!peerPort) {
        throw std::runtime_error(fmt::format("Can't find port {}", peer));
    }

    auto newPortName = fmt::format("in_{}", m_pairs.size());
    auto newPort     = jack_port_register(m_client, newPortName.c_str(), JACK_DEFAULT_AUDIO_TYPE,
                                          JackPortIsInput, 0);

    if (!newPort) {
        throw std::runtime_error("Could not register new port");
    }
    #ifdef DEBUG
        fmt::print("Successfully registered port {}\n", newPortName);
    #endif
    m_pairs.push_back(std::make_pair(std::make_pair(peerPort, newPort), processor));
}


JackClient::PortSet JackClient::GetOutPorts(const std::vector<std::string>& filter, bool connectedOnly) {
    PortSet result;
    auto try_insert = [this, &result, connectedOnly](auto ports) {
        for (unsigned i = 0; ports && ports[i]; ++i) {
            if (connectedOnly) {
                auto portObj = jack_port_by_name(m_client, ports[i]);
                if (!portObj)
                    continue;
                auto connections = jack_port_get_all_connections(m_client, portObj);
                if (!connections)
                    continue;
                jack_free(connections);
            }
            result.insert(ports[i]);
        }
        jack_free(ports);
    };

    if (filter.empty()) {
        auto ports = jack_get_ports(m_client, NULL, JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);
        try_insert(ports);
    } else {
        for (const auto& pattern : filter) {
            auto ports = jack_get_ports(m_client, pattern.c_str(), JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput);
            try_insert(ports);
        }
    }
    return result;
}

void JackClient::PlayWavFile(const std::string& filePath) {
    if (loadWavFile(filePath) != 0) {
        fmt::print(stderr, "Failed to play WAV file: {}\n", filePath);
    }
}

int JackClient::loadWavFile(const std::string& filename) {
    memset(&wavInfo, 0, sizeof(wavInfo));

    wavFile = sf_open(filename.c_str(), SFM_READ, &wavInfo);
    if (!wavFile) {
        fmt::print(stderr, "Failed to open audio file: {}\n", filename);
        return 1;
    }
    #ifdef DEBUG
        fmt::print("Playing WAV file: {}\n", filename);
        fmt::print("Sample rate: {}, Channels: {}, Frames: {}\n", wavInfo.samplerate, wavInfo.channels, wavInfo.frames);
    #endif
    wavBuffer.resize(wavInfo.frames * wavInfo.channels);
    sf_count_t readcount = sf_read_float(wavFile, wavBuffer.data(), wavBuffer.size());
    if (readcount != wavInfo.frames * wavInfo.channels) {
        fmt::print(stderr, "Failed to read audio data from file\n");
        return 1;
    }

    return 0;
}

int JackClient::OnJackProcess(jack_nframes_t nFrames, void *arg)
{
    JackClient *self = static_cast<JackClient *>(arg);
    #ifdef DEBUG
        fmt::print("OnJackProcess called with {} frames\n", nFrames);
    #endif
    // Process WAV file if loaded
    if (self->wavFile)
    {
        float *buffer = static_cast<float *>(jack_port_get_buffer(self->m_pairs[0].first.second, nFrames));
        
        if (!buffer)
        {
            fmt::print(stderr, "Error: buffer is null\n");
            return -1;  // Return error if the buffer is null
        }

        // Process WAV data and copy it into the buffer
        self->ProcessWavData(buffer, nFrames);

        // Pass the processed WAV buffer to the peak processor
        for (auto &pair : self->m_pairs)
        {
            pair.second->Process(buffer, nFrames);

            // Cast the processor to SimplePeakProcessor
            auto peakProcessor = std::dynamic_pointer_cast<SimplePeakProcessor>(pair.second);
            #ifdef DEBUG
                if (peakProcessor && peakProcessor->SignalDetected()) 
                {
                    fmt::print("WAV Signal detected: Latest Peak = {}\n", peakProcessor->GetLatestPeak());
                }
            #endif
        }
    }
    else
    {
        // Process real inputs/outputs
        try
        {
            for (auto &pair : self->m_pairs) {
                auto buffer = static_cast<float *>(jack_port_get_buffer(pair.first.second, nFrames));
                
                pair.second->Process(buffer, nFrames);
            }
        }
        catch (const std::exception &ex)
        {
            fmt::print(stderr, "Caught exception in JACK process callback: {}\n", ex.what());
            return -1;
        }
    }
    return 0;
}

void JackClient::ProcessWavData(float *buffer, size_t frames)
{
    if (wavBuffer.size() >= frames)
    {
        std::copy(wavBuffer.begin(), wavBuffer.begin() + frames, buffer);
        wavBuffer.erase(wavBuffer.begin(), wavBuffer.begin() + frames);

        #ifdef DEBUG
            fmt::print("Processing WAV data: ");
            for (size_t i = 0; i < std::min(size_t(10), frames); ++i) {
            fmt::print("{}, ", buffer[i]);
            }
            mt::print("\n");
        #endif
        
        // Call the SimplePeakProcessor to process the buffer
        m_peakProcessor.Process(buffer, static_cast<uint32_t>(frames));

        #ifdef DEBUG
            if (m_peakProcessor.SignalDetected()) 
            {
                fmt::print("Signal detected: Latest Peak = {}\n", m_peakProcessor.GetLatestPeak());
            }
        #endif
    }
       
    else
    {
        std::fill(buffer, buffer + frames, 0.0f);

        if (!processingComplete)  
        {
            processingComplete = true;  
        }
    }
}

bool JackClient::NaturalStringLess::operator()(const std::string& a, const std::string& b) const {
    std::string first = a + '$';
    std::string second = b + '$';
    const auto size = std::min(first.length(), second.length());

    for (size_t i = 0; i < size; ++i) {
        if (isdigit(first[i]) && isdigit(second[i])) {
            for (size_t n = i; n < size; ++n) {
                if (isdigit(first[n]) != isdigit(second[n])) {
                    return isdigit(second[n]);
                }
                if (!isdigit(first[n]))
                    break;
            }
        }
        if (first[i] != second[i]) {
            if (first[i] == '-' && isdigit(second[i]))
                return true;
            return first[i] < second[i];
        }
    }
    return first.length() < second.length();
}

void JackClient::NaturalSort(std::vector<std::string>& data) {
    std::sort(data.begin(), data.end(), NaturalStringLess{});
}

} // namespace jackmeter
