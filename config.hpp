#pragma once
#include "api.hpp"
#include <thread>

namespace app
{
    struct settings
    {
        uint32_t threads = std::thread::hardware_concurrency();
        std::string root_dir{};
        api::change_rules pattern_shift{};
    };

    class config
    {
    protected:
        config() = default;
        void add_setting(std::string& key, const std::string& val);

    public:
        explicit config(const std::string& config_path);
    
        void parse(const std::string& config_path);
        void short_description();
        
        std::string get_root() const { return _setting.root_dir; }
        uint32_t get_threads_amount() const { return _setting.threads; }
        api::change_rules get_pattern() const { return _setting.pattern_shift; }

    protected:
        settings _setting;
    };
}
