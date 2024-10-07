#pragma once

#include "Config.hpp"
#include "IProcessor.hpp"
#include "SimplePeakProcessor.hpp"
#include <algorithm>
#include <jack/jack.h>
#include <memory>
#include <set>
#include <sndfile.h>
#include <string>
#include <string_view>
#include <vector>

namespace jackmeter {

class JackClient {
public:
    struct NaturalStringLess {
        bool operator()(const std::string& a, const std::string& b) const;
    };

    using PortSet = std::set<std::string, NaturalStringLess>;

    explicit JackClient(std::string_view name, Config& config);
    virtual ~JackClient();

    virtual void Activate();
    virtual void Deactivate();
    virtual void Connect();
    void AddProbe(std::string_view peer, std::shared_ptr<IProcessor> processor);
    PortSet GetOutPorts(const std::vector<std::string>& filter, bool connectedOnly);

    void PlayWavFile(const std::string& filePath);
    int loadWavFile(const std::string& filename);
    bool processingComplete = false; 
private:
    std::string m_clientName;
    jack_client_t* m_client;
    jack_nframes_t m_bufSize;
    jack_nframes_t m_sampleRate;
    Config& m_config;
    jackmeter::SimplePeakProcessor m_peakProcessor;

    using Link = std::pair<jack_port_t*, jack_port_t*>;
    using ProcessingPair = std::pair<Link, std::shared_ptr<IProcessor>>;
    using ProcessingPairs = std::vector<ProcessingPair>;
    ProcessingPairs m_pairs;

    SNDFILE* wavFile;
    SF_INFO wavInfo;
    std::vector<float> wavBuffer;

    void ProcessWavData(float* buffer, size_t frames);
    static int OnJackProcess(jack_nframes_t nFrames, void* arg);
    void NaturalSort(std::vector<std::string>& data);
};

} // namespace jackmeter
