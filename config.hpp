#pragma once

#include "api.hpp"
#include <thread>

namespace app
{
    struct settings
    {
        int threads = std::thread::hardware_concurrency();
        std::string root_dir{};
        api::change_rules pattern_shift{};
    };

    class config
    {
    public:
        explicit config(const std::string& config_path);
        void parse(const std::string& config_path);
        void short_description();
        
        std::string get_root() const { return setting.root_dir; }
        int get_threads_amount() const { return setting.threads; }
        api::change_rules get_pattern() const { return setting.pattern_shift; }

    private:
        void add_setting(std::string& key, const std::string& val);

    private:
        settings setting;
    };
}
