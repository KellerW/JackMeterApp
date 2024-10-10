#pragma once
#include "AppContext.hpp"
#include <memory>

class SignalHandler {
public:
    SignalHandler(std::shared_ptr<AppContext> appContext);

    // Static signal handler function
    static void handle_signal(int signum);

private:
    static std::shared_ptr<AppContext> context;
};
