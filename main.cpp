#include <ncurses.h>
#include <csignal>
#include <getopt.h>
#include <memory>
#include <vector>
#include <poll.h>
#include <thread>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <thread>
#include <chrono>
#include <sys/types.h>
#include "CommandInvoker.h"
#include "Commands.hpp"
#include "Config.hpp"
#include "JackClient.hpp"
#include "SimplePeakProcessor.hpp"
#include "DiscontinuityDetector.hpp"

// Global variables
Config config;
std::shared_ptr<jackmeter::JackClient> client;
std::vector<std::shared_ptr<jackmeter::SimplePeakProcessor>> processors;
WINDOW* window = nullptr;  // Ncurses window
const char* dB_labels[] = {"+5", "0", "-5", "-10", "-20", "-30", "-40", "-50"};
int level_positions[] = {1, 20, 40, 60, 100, 120, 140, 160};  

// Cleanup ncurses before exit
void cleanup_ncurses() 
{
    if (!config.plain_output && window != nullptr) 
    {
        endwin();  // Clean up ncurses
        window = nullptr;
    }
}

// Signal handler for safe shutdown
void signal_callback_handler(int signum)
{
    if (!config.plain_output) 
    {
        cleanup_ncurses();
    }
    client.reset();  // Reset the client safely
    exit(signum);    // Terminate the program with the received signal
}

// Function to log current PPM and peak values to a file
void log_values(const char* processor_name, float currentPPM, float peakHold) 
{
    FILE* log_file = fopen("ppm_log.txt", "a");
    if (log_file != NULL) {
        fprintf(log_file, "%s : Current PPM: %6.1f dB  Peak Hold: %6.1f dB\n", processor_name, currentPPM, peakHold);
        fclose(log_file);
    } else {
        fprintf(stderr, "Error: Unable to open log file.\n");
    }
}

// Ncurses initialization for terminal UI
void initialize_ncurses() 
{
    if (!config.plain_output) 
    {
        initscr();      // Initialize the ncurses screen
        cbreak();       // Disable line buffering
        noecho();       // Do not echo typed characters
        curs_set(FALSE);
        nodelay(stdscr, TRUE); // Non-blocking input
        window = stdscr; // Use stdscr as the main window
    }
}

// Map dB value to meter scale (0 to MAX_LEVEL)
int map_db_to_level(float dB, int MAX_LEVEL) {
    float max_dB = 5.0f;    // Max dB value corresponding to top of the meter
    float min_dB = -50.0f;  // Min dB value corresponding to bottom of the meter

    if (std::isinf(dB)) return 0;  // Return 0 for silent (-inf) signals
    if (dB > max_dB) dB = max_dB;  // Clamp to max dB
    if (dB < min_dB) dB = min_dB;  // Clamp to min dB

    // Scale dB to match the meter's height (MAX_LEVEL)
    return (int)((dB - min_dB) / (max_dB - min_dB) * MAX_LEVEL);
}

// Ncurses-based function to print a horizontal meter with dB markers
void print_meter_ncurses(WINDOW* win, int level, int peak_level, int max_level, const std::string& label) {
    // Non-linear dB markers with adjusted spacing
    const int db_positions[] = {0, 10, 20, 30, 40};  // Mapped to +5, 0, -10, -20, -40
    const char* db_labels[] = {"+5", "0", "-10", "-20", "-40"};

    // Print the label
    wprintw(win, "%-6s |", label.c_str());

    // Print the meter scale
    for (int i = 0; i < max_level; i++) {
        if (i < level) {
            waddch(win, '|');  // Bar for each level
        } else {
            waddch(win, ' ');  // Space for empty parts of the bar
        }
    }

    // Peak hold indicator
    if (peak_level < max_level) {
        mvwaddch(win, getcury(win), 8 + peak_level, '^');
    }

    waddch(win, '|');  // End of bar
    wprintw(win, "\n");

    // Print dB markers below the meter
    wprintw(win, "%-6s |", "dB ");
    for (int i = 0; i < max_level; i++) {
        bool printed = false;
        for (int j = 0; j < 5; j++) {
            if (i == db_positions[j]) {
                wprintw(win, "%-4s", db_labels[j]);
                printed = true;
                break;
            }
        }
        if (!printed) {
            waddch(win, ' ');
        }
    }
    waddch(win, '|');
    wprintw(win, "\n");
}

// Function to map dB value to a non-linear meter scale
int map_db_to_non_linear_level(float dB, int MAX_LEVEL) {
    // Define non-linear dB ranges and map them to meter positions
    if (dB >= 5.0f) return 0;       // +5 dB
    if (dB >= 0.0f) return 10;      // 0 dB
    if (dB >= -10.0f) return 20;    // -10 dB
    if (dB >= -20.0f) return 30;    // -20 dB
    if (dB >= -40.0f) return 40;    // -40 dB
    return MAX_LEVEL;               // Below -40 dB
}

// Função para desenhar o gráfico PPM dinamicamente
void draw_ppm_meter_ncurses(float left_ppm, float right_ppm, float left_peak, float right_peak, int MAX_LEVEL) {
    int left_level = map_db_to_non_linear_level(left_ppm, MAX_LEVEL);
    int right_level = map_db_to_non_linear_level(right_ppm, MAX_LEVEL);
    int left_peak_level = map_db_to_non_linear_level(left_peak, MAX_LEVEL);
    int right_peak_level = map_db_to_non_linear_level(right_peak, MAX_LEVEL);

    // Limpa a janela
    wclear(stdscr);

    // Imprime o cabeçalho do gráfico
    wprintw(stdscr, "PPM Meter:\n");

    // Imprime os gráficos para os canais esquerdo e direito
    print_meter_ncurses(stdscr, left_level, left_peak_level, MAX_LEVEL, "Left ");
    print_meter_ncurses(stdscr, right_level, right_peak_level, MAX_LEVEL, "Right");

    // Atualiza a janela
    wrefresh(stdscr);
}

// Function to log status in plain text mode
void log_status_plain()
{
    time_t rawtime;
    struct tm* ltime;
    time(&rawtime);
    ltime = localtime(&rawtime);

    printf("# jackmeter %04u-%02u-%02ut%02u:%02u:%02u\n", 1900 + ltime->tm_year, 1 + ltime->tm_mon, ltime->tm_mday, ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
    for (const auto& processor : processors) {
        printf("%s : %6.1f db  (min:%6.1f, max:%6.1f)\n", processor->GetName().data(), processor->GetLatestPeakDb(), processor->GetMinPeakDb(), processor->GetMaxPeakDb());
    }
    fflush(stdout);
}

// Check if stdin is available
bool stdin_available()
{
    struct pollfd pfd[1] = {};
    pfd[0].fd = fileno(stdin);
    pfd[0].events = POLLIN;
    int ret = poll(pfd, 1, 0);
    if (ret > 0) {
        if (getc(stdin) == EOF)
            return false;
    }
    return ret > 0;
}

// Parse command line arguments and execute commands
void parse_command_line_arguments(int argc, char *argv[], jackmeter::invoker::CommandInvoker& invoker) {
    int opt;
    while ((opt = getopt(argc, argv, "f:d:n:w:pcxh")) != -1)
    {
        switch (opt)
        {
            case 'f':
                invoker.addCommand(std::make_shared<jackmeter::commands::FrequencyCommand>(config, optarg));
                break;
            case 'd':
                invoker.addCommand(std::make_shared<jackmeter::commands::DurationCommand>(config, optarg));
                break;
            case 'n':
                invoker.addCommand(std::make_shared<jackmeter::commands::MaxCountCommand>(config, optarg));
                break;
            case 'c':
                invoker.addCommand(std::make_shared<jackmeter::commands::ConnectedOnlyCommand>(config));
                break;
            case 'p':
                invoker.addCommand(std::make_shared<jackmeter::commands::PlainOutputCommand>(config));
                break;
            case 'x':
                invoker.addCommand(std::make_shared<jackmeter::commands::NoMatchingCommand>(config));
                break;
            case 'h':
                invoker.addCommand(std::make_shared<jackmeter::commands::HelpCommand>());
                break;
            case 'w':
                invoker.addCommand(std::make_shared<jackmeter::commands::LoadWaveFile>(optarg, config));
                break;
            default:
                invoker.addCommand(std::make_shared<jackmeter::commands::UsageCommand>());
        }
    }
}

// Add audio ports to the JackClient
void add_ports_to_client(const std::vector<std::string>& ports) 
{
    for (const auto& portToListenTo : ports) 
    {
        auto processor = std::make_shared<jackmeter::SimplePeakProcessor>(portToListenTo, 0.05f);
        processors.push_back(processor);
        client->AddProbe(portToListenTo, processor);
    }
}

// Main function
int main(int argc, char *argv[])
{
    int MAX_LEVEL = 50;  // Widened the meter to fit more variation
    float left_ppm = -5.0f, right_ppm = -3.0f;
    float left_peak = -3.0f, right_peak = -2.5f;
    
    signal(SIGINT, signal_callback_handler);  // Handle Ctrl+C

    jackmeter::invoker::CommandInvoker invoker;
    initialize_ncurses();
    
    parse_command_line_arguments(argc, argv, invoker);
    invoker.executeCommands();

    client = std::make_shared<jackmeter::JackClient>("jackmeter_client", config);

    if (!config.wav_file.empty()) {
        try {
            auto processor1 = std::make_shared<jackmeter::SimplePeakProcessor>("system:capture_1", 0.05f);
            auto processor2 = std::make_shared<jackmeter::SimplePeakProcessor>("system:capture_2", 0.05f);
            client->AddProbe("system:capture_1", processor1);
            client->AddProbe("system:capture_2", processor2);
            processors.push_back(processor1);
            processors.push_back(processor2);
            client->PlayWavFile(config.wav_file);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            cleanup_ncurses();
            return EXIT_FAILURE;
        }
    } else {
        std::vector<std::string> filter;
        for (; argc > optind; optind++) {
            filter.emplace_back(std::string(argv[optind]));
        }

        if (config.no_matching) {
            add_ports_to_client(filter);
        } else {
            auto ports = client->GetOutPorts(filter, config.connected_only);
            std::vector<std::string> ports_vector(ports.begin(), ports.end());
            add_ports_to_client(ports_vector);
        }

        if (processors.empty()) {
            cleanup_ncurses();
            return EXIT_FAILURE;  // Exit if no ports to process
        }
    }

    client->Activate();
    client->Connect();
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while (!client->processingComplete && !stdin_available()) 
    {
        if (config.plain_output) {
            log_status_plain();
        } else {
            // Draw the PPM meter using ncurses
            for (const auto& processor : processors) {
                float currentPPM = processor->GetCurrentPPMDb();
                float peakHold = processor->GetPeakHoldDb();

                draw_ppm_meter_ncurses(currentPPM,currentPPM , peakHold, peakHold, MAX_LEVEL);
                wrefresh(window);  // Refresh ncurses window
                log_values("AudioProcessor1", currentPPM, peakHold);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds { static_cast<uint32_t>(1000 / config.rate) });
    }

    cleanup_ncurses();
    return EXIT_SUCCESS;
}
