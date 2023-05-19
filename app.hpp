#pragma once
#include "config.hpp"
#include "parser.hpp"
#include "threads.hpp"

//TODO: add simple unit tests

namespace app
{
    class application
    {
    public:
        explicit application(const std::string& config_path);
        ~application() = default;

        void start();

    private:
        app::config _config;
        std::shared_ptr<app::core::parser> _parser;
        app::core::thread_pool _th_pool;
    };
}
