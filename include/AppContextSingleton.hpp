#pragma once 
#include <memory>
#include "AppContext.hpp"

class AppContextSingleton {
public:
    static std::shared_ptr<AppContext> get_instance() {
        static std::shared_ptr<AppContext> instance = std::make_shared<AppContext>();
        return instance;
    }

private:
    AppContextSingleton() = default;  // Private constructor to prevent direct instantiation
};
