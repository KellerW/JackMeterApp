#pragma once

#include "Config.hpp"
#include <fmt/format.h>

namespace jackmeter
{
namespace commands
{

constexpr char Usage[] = "Usage: jackmeter [-h] [-c] [-p] [-x] [-f <freq>] [-d <seconds>] [-n "
                         "<measurements> ] [<port>, ...]";
class Command
{
  public:
    virtual ~Command() = default; 
    virtual void execute() = 0;
};

// FrequencyCommand
class FrequencyCommand : public Command
{
  private:
    Config &config;
    const char *optarg;

  public:
    FrequencyCommand(Config &config, const char *optarg);
    void execute() override;
};

// DurationCommand
class DurationCommand : public Command
{
  private:
    Config &config;
    const char *optarg;

  public:
    DurationCommand(Config &config, const char *optarg);
    void execute() override;
};

// MaxCountCommand
class MaxCountCommand : public Command
{
  private:
    Config &config;
    const char *optarg;

  public:
    MaxCountCommand(Config &config, const char *optarg);
    void execute() override;
};

// ConnectedOnlyCommand
class ConnectedOnlyCommand : public Command
{
  private:
    Config &config;

  public:
    ConnectedOnlyCommand(Config &config);
    void execute() override;
};

// PlainOutputCommand
class PlainOutputCommand : public Command
{
  private:
    Config &config;

  public:
    PlainOutputCommand(Config &config);
    void execute() override;
};

// NoMatchingCommand
class NoMatchingCommand : public Command
{
  private:
    Config &config;

  public:
    NoMatchingCommand(Config &config);
    void execute() override;
};

// LoadWaveFile Command
class LoadWaveFile : public Command
{
  private:
    Config &config;
    const char *optarg;

  public:
    LoadWaveFile(const char *optarg, Config &config);
    void execute() override;
};

class UsageCommand : public Command
{
  public:
    UsageCommand() = default;
    void execute() override;
};

// HelpCommand
class HelpCommand : public Command
{
  public:
    HelpCommand() = default;
    void execute() override;
};

} // namespace commands
} // namespace jackmeter
