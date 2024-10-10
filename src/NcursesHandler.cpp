#include "NcursesHandler.hpp"

void NcursesHandler::initialize() 
{
    initscr();  // Initialize the ncurses screen
    cbreak();   // Disable line buffering
    noecho();   // Do not echo typed characters
    curs_set(FALSE);
    nodelay(stdscr, TRUE);  // Non-blocking input
    context->window = stdscr;  // Store the ncurses window in the AppContext
}

void NcursesHandler::cleanup() 
{
    if (context->window != nullptr) {
        endwin();  // Clean up ncurses
        context->window = nullptr;  // Reset the window pointer in AppContext
    }
}

void NcursesHandler::log_status(const std::string& processor_name, float peak, float min_peak, float max_peak) 
{
    // Log the status using the window from AppContext
    wprintw(context->window, "%s : %6.1f db  (min:%6.1f, max:%6.1f)\n", processor_name.c_str(), peak, min_peak, max_peak);
    wrefresh(context->window);  // Refresh ncurses window
}