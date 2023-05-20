#pragma once
#include <unordered_set>
#include <vector>
#include <memory>
#include "config.hpp"

namespace app::core
{
    class thread_pool;

    class parser : public std::enable_shared_from_this<parser>
    {
    public:
        explicit parser(const app::config& config, app::core::thread_pool& th_pool);
        ~parser() = default;

        void show_paths();
        void change_by_rules();

    protected:
        void parse_files_paths();
        void change_file(const api::file_path& file, const api::change_rules& rules);
        void rewrite_file(const api::file_path& file, const std::vector<std::string>& file_data);
        void handle_line(std::string& line, const api::change_rules& rules);

    protected:
        const app::config& _config;
        app::core::thread_pool& _th_pool;
        std::unordered_set<api::file_path> _files;
    };
}
