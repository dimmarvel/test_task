#include "config.hpp"
#include <sstream>
#include <iostream>
#include <fstream>
#include <algorithm>

namespace app
{
    config::config(const std::string& config_path)
    {
        parse(config_path);
    }

    void config::parse(const std::string& config_path)
    {
        std::ifstream file(config_path);

        if (!file)
        {
            std::cout << "Error: when parse config file" << std::endl;
            std::exit(EXIT_FAILURE);
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        std::string line;
        while(std::getline(buffer, line))
        {
            std::istringstream is_line(line);
            std::string key;
            if(std::getline(is_line, key, '='))
            {
                std::string value;
                if(std::getline(is_line, value))
                {
                    add_setting(key, value);
                }
            }
        }
    }

    void config::add_setting(std::string& key, const std::string& val)
    {
        if(key == "threads")
        {
            int t = std::stoi(val);
            if(t == 0)
            {
                std::cout << "WARN: the number of threads has been increased because it is set to zero" << std::endl;
                return;
            }

            if(t > std::thread::hardware_concurrency())
            {
                std::cout << "WARN: the number of threads has been reduced to the supported number" << std::endl;
                return;
            }

            setting.threads = t;
            return;
        }

        if(key == "root_dir")
        {
            setting.root_dir = val;
            return;
        }

        if( (key.at(0) == '<' && key.at(1) == '%') &&
            (key.at(key.size() - 2) == '%' && key.at(key.size() - 1) == '>'))
        {
            if(setting.pattern_shift.find(key) != setting.pattern_shift.end())
            {
                std::cout << "WARN: find same template - " << key << ", " << val << std::endl;
            }
            setting.pattern_shift.insert({key, val});
            return;
        }

        std::cout << "ERROR: ivalid config file - " << key << ", " << val << std::endl;
        std::exit(EXIT_FAILURE);
    }

    void config::short_description()
    {
        std::cout << "------Config------" << std::endl;
        std::cout << "Threads = " << setting.threads << std::endl
                  << "Root dir path = " << setting.root_dir << std::endl
                  << "Templates:" << std::endl;
                  
        for(const auto& it : setting.pattern_shift)
            std::cout << it.first << " -> " << it.second << std::endl;

        std::cout << "------------------" << std::endl;
    }
}
