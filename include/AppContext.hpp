#pragma once

#include "Config.hpp"
#include "FFTProcessor.hpp"
#include "IProcessor.hpp"
#include "JackClient.hpp"
#include "SimplePeakProcessor.hpp"
#include <memory>
#include <mutex>
#include <ncurses.h>
#include <vector>

struct AppContext
{
    Config config;
    std::shared_ptr<jackmeter::JackClient> client;
    std::vector<std::shared_ptr<jackmeter::IProcessor>> processors;
    WINDOW *window = nullptr;
    std::mutex processors_mutex; // Protects processors
    std::mutex client_mutex;     // Protects client
    void log_status_plain();
};