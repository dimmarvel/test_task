#include "parser.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <functional>

namespace app::core
{
    namespace fs = std::filesystem;

    parser::parser(const app::config& config, app::core::thread_pool& th_pool)
    :
    _config(config),
    _th_pool(th_pool)
    {
        parse_files_paths();
    }    

    void parser::parse_files_paths()
    {
        for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(_config.get_root()))
            if(!dirEntry.is_directory())
                _files.insert(dirEntry.path());
    }   

    void parser::change_by_rules()
    {
        for(const auto& file : _files)
        {
            std::cout << "Process file " << file << std::endl;
            _th_pool.add_task(std::bind(&parser::change_file, shared_from_this(), file, _config.get_pattern()));
        }
        _th_pool.wait_all();
    }

    void parser::change_file(const api::file_path& file, const api::change_rules& rules)
    {
        std::ifstream fin;

        fin.open(file);
        if (!fin.is_open())
        {
            std::cout << "ERROR: file " << file << " not found" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        std::vector<std::string> file_data; 
        std::string line;
        while (getline(fin, line, '\n'))
        {   
            handle_line(line, rules);
            file_data.push_back(std::move(line));
        }
        fin.close();

        rewrite_file(file, file_data);
    }

    void parser::rewrite_file(const api::file_path& file, const std::vector<std::string>& file_data)
    {
        std::ofstream fout(file);
        for(auto it : file_data)
            fout << it << '\n';
        fout.close();
    }

    void parser::handle_line(std::string& line, const api::change_rules& rules)
    {
        int open_pos = 0;
        while(open_pos != std::string::npos)
        {
            open_pos = line.find("<%", (open_pos != 0) ? open_pos + 1 : open_pos);
            if(open_pos == std::string::npos) 
                break;

            int end_pos = line.find("%>", open_pos + 1); // TODO: add a constraint to search up to the longest pattern
            if(end_pos == std::string::npos)
                continue;
            
            std::string pattern = line.substr(open_pos, (end_pos - open_pos) + 2);
            if(rules.find(pattern) != rules.end())
            {
                line.replace(open_pos, pattern.size(), rules.at(pattern));
            }
        }
    }

    void parser::show_paths()
    {
        for(const auto& it : _files)
            std::cout << it << std::endl;
    }
}
