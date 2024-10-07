#pragma once
#include <string>
struct Config
{
    int rate            = 8;     // Default update frequency
    int duration        = 0;     // Default duration (infinite)
    int max_count       = 0;     // Default max measurements (infinite)
    bool connected_only = false; // Monitor connected ports only
    bool plain_output   = false; // Use plain output instead of ncurses
    bool no_matching    = false; // Disable pattern matching and sorting
    std::string wav_file;        // WAV file path
};
