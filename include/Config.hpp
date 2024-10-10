#pragma once
#include <string>
struct Config
{
    int rate            = 8;     
    int duration        = 5;     
    int max_count       = 0;     
    bool connected_only = false; 
    bool plain_output   = false; 
    bool no_matching    = false; 
    double th           = 0.001f;
    std::string wav_file; 
    bool print_ppm = false;
    bool print_fft = false;
    bool print_dt = false;
};
