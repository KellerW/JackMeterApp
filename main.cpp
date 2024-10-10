#include "AppContext.hpp"
#include "AppContextSingleton.hpp"
#include "CommandInvoker.hpp"
#include "GnuplotHandler.hpp"
#include "JackClient.hpp"
#include "NcursesHandler.hpp"
#include "SimplePeakProcessor.hpp"
#include <csignal>
#include <getopt.h>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <poll.h>
#include <sys/types.h>
#include <thread>
#include <vector>
#include "SignalHandler.hpp"
#include "Utils.hpp"
// Global AppContext to ensure consistency in signal handling
std::shared_ptr<AppContext> g_app_context = nullptr;
GnuplotHandler gnuplot_handler;

// Parse command line arguments and add commands
void parse_command_line_arguments(int argc, char *argv[],
                                  jackmeter::invoker::CommandInvoker &invoker,
                                  std::shared_ptr<AppContext> ctx)
{
    int opt;
    while ((opt = getopt(argc, argv, "t:f:g:d:n:w:pcxh")) != -1)
    {
        switch (opt)
        {
            case 'f':
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::FrequencyCommand>(ctx->config, optarg));
                break;
            case 'd':
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::DurationCommand>(ctx->config, optarg));
                break;
            case 'n':
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::MaxCountCommand>(ctx->config, optarg));
                break;
            case 'c':
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::ConnectedOnlyCommand>(ctx->config));
                break;
            case 'p':
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::PlainOutputCommand>(ctx->config));
                break;
            case 'x':
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::NoMatchingCommand>(ctx->config));
                break;
            case 'h':
                invoker.addCommand(std::make_shared<jackmeter::commands::HelpCommand>());
                break;
            case 'w':
                invoker.addCommand(
                    std::make_shared<jackmeter::commands::LoadWaveFile>(optarg, ctx->config));
                break;
            case 'g':
                invoker.addCommand(std::make_shared<jackmeter::commands::PrintPPM>(ctx->config));
                break;
            case 't':
                invoker.addCommand(std::make_shared<jackmeter::commands::PrintFFT>(ctx->config));
                break;
            case 'b':
                invoker.addCommand(std::make_shared<jackmeter::commands::PrintDT>(ctx->config));
                break;
            default:
                invoker.addCommand(std::make_shared<jackmeter::commands::UsageCommand>());
        }
    }
}

// Add ports to JACK client
void add_ports_to_client(const std::vector<std::string> &ports, std::shared_ptr<AppContext> ctx)
{
    for (const auto &port_to_listen_to : ports)
    {
        auto processor = std::make_shared<jackmeter::SimplePeakProcessor>(port_to_listen_to, 0.05f);

        // Lock processors vector for thread safety
        std::lock_guard<std::mutex> lock(ctx->processors_mutex);
        ctx->processors.push_back(processor);

        // Lock client for thread safety
        std::lock_guard<std::mutex> client_lock(ctx->client_mutex);
        ctx->client->AddProbe(port_to_listen_to, processor);
    }
}

int main(int argc, char *argv[])
{
    // Get the singleton instance of AppContext
    g_app_context = AppContextSingleton::get_instance();

    // Initialize the SignalHandler with the AppContext
    SignalHandler signalHandler(g_app_context);
   
    signal(SIGINT, SignalHandler::handle_signal);


    NcursesHandler ncurses_handler(g_app_context);

    // Initialize ncurses
    ncurses_handler.initialize();

    // Parse command line arguments
    jackmeter::invoker::CommandInvoker invoker;
    parse_command_line_arguments(argc, argv, invoker, g_app_context);
    invoker.executeCommands();

    // Initialize JACK client
    g_app_context->client =
        std::make_shared<jackmeter::JackClient>("jackmeter_client", g_app_context->config);

    // Handle WAV file playback
    if (!g_app_context->config.wav_file.empty())
    {
        try
        {
            auto processor1 =
                std::make_shared<jackmeter::SimplePeakProcessor>("system:capture_1", 0.05f);
          //  auto processor2 =
          //      std::make_shared<jackmeter::SimplePeakProcessor>("system:capture_2", 0.05f);

            // Create FFT Processors
            auto fftProcessor1 =
                std::make_shared<jackmeter::FFTProcessor>(1024); // Example buffer size
          //  auto fftProcessor2 =
         //       std::make_shared<jackmeter::FFTProcessor>(1024); // Example buffer size
            
            auto dtProcessor1 =
                std::make_shared<jackmeter::FFTProcessor>(1024); // Example buffer size
         //   auto dtProcessor2 =
          //      std::make_shared<jackmeter::FFTProcessor>(1024); // Example buffer size

            // Add the processors (SimplePeakProcessor and FFTProcessor) to the JACK client
            g_app_context->client->AddProbe("system:capture_1", processor1);
           // g_app_context->client->AddProbe("system:capture_2", processor2);
            g_app_context->client->AddProbe("system:capture_1", fftProcessor1);
          //  g_app_context->client->AddProbe("system:capture_2", fftProcessor2);
            g_app_context->client->AddProbe("system:capture_1", dtProcessor1);
         //   g_app_context->client->AddProbe("system:capture_2", dtProcessor2);

            // Store all processors in the AppContext (both SimplePeakProcessor and FFTProcessor)
            g_app_context->processors.push_back(processor1);
        //    g_app_context->processors.push_back(processor2);
            g_app_context->processors.push_back(fftProcessor1);
         //   g_app_context->processors.push_back(fftProcessor2);
            g_app_context->processors.push_back(dtProcessor1);
         //   g_app_context->processors.push_back(dtProcessor2);

            g_app_context->client->PlayWavFile(g_app_context->config.wav_file);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            ncurses_handler.cleanup();
            return EXIT_FAILURE;
        }
    }
    else
    {
        std::vector<std::string> filter;
        for (; argc > optind; optind++)
        {
            filter.emplace_back(std::string(argv[optind]));
        }

        if (g_app_context->config.no_matching)
        {
            add_ports_to_client(filter, g_app_context);
        }
        else
        {
            auto ports =
                g_app_context->client->GetOutPorts(filter, g_app_context->config.connected_only);
            std::vector<std::string> ports_vector(ports.begin(), ports.end());
            add_ports_to_client(ports_vector, g_app_context);
        }

        if (g_app_context->processors.empty())
        {
            ncurses_handler.cleanup();
            return EXIT_FAILURE; // Exit if no ports to process
        }
    }

    // Activate and connect the JACK client
    {
        std::lock_guard<std::mutex> lock(g_app_context->client_mutex);
        g_app_context->client->Activate();
        g_app_context->client->Connect();
    }

    // Main processing loop
auto start      = std::chrono::steady_clock::now();
auto stop       = start + std::chrono::seconds(g_app_context->config.duration);
int measurement = 0;

while (
    !g_app_context->client->processingComplete && !stdin_available() &&
    (std::chrono::steady_clock::now() < stop || g_app_context->config.duration == 0) &&
    (++measurement <= g_app_context->config.max_count || g_app_context->config.max_count == 0))
{
    if (g_app_context->config.plain_output)
    {
       g_app_context->log_status_plain();
    }
    else
    {
        // Lock processors vector for thread safety
        std::lock_guard<std::mutex> lock(g_app_context->processors_mutex);
        for (const auto &processor : g_app_context->processors)
        {
            // Check if the processor is a SimplePeakProcessor
            if (auto peakProcessor =
                    std::dynamic_pointer_cast<jackmeter::SimplePeakProcessor>(processor))
            {
                wprintw(g_app_context->window, "%s : %6.1f db  (min:%6.1f, max:%6.1f)\n",
                        std::string(peakProcessor->GetName()).c_str(),
                        peakProcessor->GetLatestPeakDb(), peakProcessor->GetMinPeakDb(),
                        peakProcessor->GetMaxPeakDb());
                wrefresh(g_app_context->window);

                // If enabled, plot the PPM for peak data
                if (g_app_context->config.print_ppm)
                {
                    gnuplot_handler.plot_with_gnuplot_ppm(peakProcessor->GetLatestPeakDb());
                }
            }
            else if (auto fftProcessor =
                        std::dynamic_pointer_cast<jackmeter::FFTProcessor>(processor))
            {
                if (g_app_context->config.print_fft)
                {
                    gnuplot_handler.plot_fft(fftProcessor->GetFrequencyBins());
                }
            }
            else if (auto dtProcessor = std::dynamic_pointer_cast<jackmeter::DiscontinuityProcessor>(processor))
            {
                // Get the indices of discontinuities
                auto discontinuityIndices = dtProcessor->getDiscontinuityIndices();

                // Print the detected discontinuities
                if (!discontinuityIndices.empty())
                {
                    wprintw(g_app_context->window, "%s : Discontinuities detected at indices: ",
                            std::string(dtProcessor->GetName()).c_str());
                    for (int index : discontinuityIndices)
                    {
                        wprintw(g_app_context->window, "%d ", index);
                    }
                    wprintw(g_app_context->window, "\n");
                }
                else
                {
                    wprintw(g_app_context->window, "%s : No discontinuities detected.\n",
                            std::string(dtProcessor->GetName()).c_str());
                }
                wrefresh(g_app_context->window);
            }
        
        }
    }
    std::this_thread::sleep_for(
        std::chrono::milliseconds{static_cast<uint32_t>(1000 / g_app_context->config.rate)});
}


    // Cleanup
    ncurses_handler.cleanup();
    return EXIT_SUCCESS;
}
