#include "Commands.hpp"

// UsageCommand
void jackmeter::commands::UsageCommand::execute() { fmt::print(stderr, "{}\n", Usage); }

// HelpCommand
void jackmeter::commands::HelpCommand::execute()
{
    fmt::print(R"({}

OPTIONS
-f      is how often to update the meter per second [8]
-d      duration seconds [infinite]
-n      n measurements [infinite]
-c      monitor only connected output ports [false]
-p      plain output [false]
-x      don't do pattern matching and don't sort
<port>  the port(s) to monitor, partial matching and with regex support
        if no ports are given, displays all ports
)",
               Usage);
}

// FrequencyCommand
jackmeter::commands::FrequencyCommand::FrequencyCommand(Config &config, const char *optarg)
    : config(config), optarg(optarg)
{
}

void jackmeter::commands::FrequencyCommand::execute()
{
    config.rate = atoi(optarg); // Store in config
    fmt::print("Rate updated to: {}\n", config.rate);
}

// DurationCommand
jackmeter::commands::DurationCommand::DurationCommand(Config &config, const char *optarg)
    : config(config), optarg(optarg)
{
}

void jackmeter::commands::DurationCommand::execute()
{
    config.duration = atoi(optarg); // Store in config
    fmt::print("Duration updated to: {}\n", config.duration);
}

// MaxCountCommand
jackmeter::commands::MaxCountCommand::MaxCountCommand(Config &config, const char *optarg)
    : config(config), optarg(optarg)
{
}

void jackmeter::commands::MaxCountCommand::execute()
{
    config.max_count = atoi(optarg); // Store in config
    fmt::print("Max count updated to: {}\n", config.max_count);
}

// ConnectedOnlyCommand
jackmeter::commands::ConnectedOnlyCommand::ConnectedOnlyCommand(Config &config) : config(config) {}

void jackmeter::commands::ConnectedOnlyCommand::execute()
{
    config.connected_only = true; // Set in config
    fmt::print("Connected only mode enabled.\n");
}

// PlainOutputCommand
jackmeter::commands::PlainOutputCommand::PlainOutputCommand(Config &config) : config(config) {}

void jackmeter::commands::PlainOutputCommand::execute()
{
    config.plain_output = true; // Set in config
    fmt::print("Plain output enabled.\n");
}

// NoMatchingCommand
jackmeter::commands::NoMatchingCommand::NoMatchingCommand(Config &config) : config(config) {}

void jackmeter::commands::NoMatchingCommand::execute()
{
    config.no_matching = true; // Set in config
    fmt::print("Pattern matching disabled.\n");
}

// LoadWaveFile Command
jackmeter::commands::LoadWaveFile::LoadWaveFile(const char *optarg, Config &config)
    : config(config), optarg(optarg) {  // Correct order
}

void jackmeter::commands::LoadWaveFile::execute()
{
    config.wav_file = std::string(optarg); // Store WAV file path in config
    fmt::print("WAV file '{}' has been loaded and saved to config.\n", config.wav_file);
}

jackmeter::commands::PrintPPM::PrintPPM(Config &config) : config(config) {}

void jackmeter::commands::PrintPPM::execute() { config.print_ppm = true; }

jackmeter::commands::PrintFFT::PrintFFT(Config &config) : config(config) {}

void jackmeter::commands::PrintFFT::execute() { config.print_fft = true; }

jackmeter::commands::PrintDT::PrintDT(Config &config) : config(config) {}

void jackmeter::commands::PrintDT::execute() { config.print_dt = true; }