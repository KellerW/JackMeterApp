#pragma once 
#include <ncurses.h>
#include <string>
#include "AppContextSingleton.hpp"  // To access the AppContext singleton

class NcursesHandler {
public:
    // Dependency Injection of AppContext via constructor
    NcursesHandler(std::shared_ptr<AppContext> ctx) : context(ctx) {}
    
    ~NcursesHandler() {
        cleanup();
    }

    void initialize();
    void cleanup();
    void log_status(const std::string& processor_name, float peak, float min_peak, float max_peak);

private:
    std::shared_ptr<AppContext> context;  // Dependency-injected context
};