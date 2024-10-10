#include "SignalHandler.hpp"
#include "NcursesHandler.hpp"
#include <cstdlib>

// Initialize the static member outside the class
std::shared_ptr<AppContext> SignalHandler::context = nullptr;

SignalHandler::SignalHandler(std::shared_ptr<AppContext> appContext)
{
    context = appContext; // Assign the context
}

void SignalHandler::handle_signal(int signum)
{
    if (context != nullptr)
    {
        if (!context->config.plain_output)
        {
            NcursesHandler(context).cleanup(); // Cleanup ncurses using AppContext
        }
        context->client.reset(); // Reset the client safely
    }
    exit(signum); // Terminate the program with the received signal
}
