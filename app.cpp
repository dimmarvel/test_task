#include "app.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>

namespace app
{
    application::application(const std::string& config_path)
    :
    _config(config_path),
    _th_pool(_config),
    _parser(std::make_shared<app::core::parser>(_config, _th_pool))
    {
        _config.short_description();
    }

    void application::start()
    {
        std::cout << "Application started" << std::endl;
        std::clock_t c_start = std::clock();
        auto t_start = std::chrono::high_resolution_clock::now();

        _parser->change_by_rules();
        std::clock_t c_end = std::clock();
        auto t_end = std::chrono::high_resolution_clock::now();
 
        std::cout << std::fixed << std::setprecision(2) 
                << "CPU time: "
                << 1000.0 * (c_end - c_start) / CLOCKS_PER_SEC << " ms\n"
                << "Default clock time passed: "
                << std::chrono::duration<double, std::milli>(t_end - t_start).count() << " ms\n";
    } 
}
