#include <iostream>
#include <functional>
#include <string>
#include <filesystem>
#include "config.hpp"
#include "parser.hpp"
#include "threads.hpp"

#define CHECK(expr1, expr2)                       \
if((expr1) == (expr2)) printf("passed\n");        \
else printf("ERROR: %s == %s \n", #expr1, #expr2) \

#define CHECK_EXPR(expr1, bad_info)                     \
if(expr1) printf("passed\n");                           \
else printf("ERROR: %s, info: %s\n", #expr1, bad_info)  \

std::string CHECK_THROWS(std::function<void()> func)
{
    try
    {
        func();
    }
    catch(const std::exception& e)
    {
        return "passed";
    }
    return "ERROR: no exception";
}

std::string CHECK_NO_THROWS(std::function<void()> func)
{
    try
    {
        func();
    }
    catch(const std::exception& e)
    {
        return "ERROR: exception - " + std::string(e.what());
    }
    return "passed";
}

class mock_parser : public app::core::parser
{
public:
    explicit mock_parser(const app::config& config, app::core::thread_pool& th_pool) : app::core::parser(config, th_pool){}
    ~mock_parser() = default;

    std::unordered_set<api::file_path> get_files() { return _files; }
};

class mock_config : public app::config
{
public:
    mock_config() = default;
    explicit mock_config(const std::string& path) : app::config(path){}
    ~mock_config() = default;

    app::settings get_settings() { return _setting; }
    void create_config(const std::string& path) { app::config cnf(path); }
    void add_setting(std::string& key, const std::string& val) { app::config::add_setting(key, val); } 
};

void config_tests()
{
    std::cout << "--------ConfigTests--------" << std::endl;
    mock_config garbage;
    std::cout << "non-existent file - " << CHECK_THROWS([&garbage](){garbage.create_config("q@#$!");}) << std::endl;

    std::string correct_path = std::filesystem::current_path().string() + "/config.txt";
    std::cout   << "existent file - "
                << CHECK_NO_THROWS([&garbage, &correct_path](){garbage.create_config(correct_path);}) << std::endl;

    mock_config mock_conf(correct_path);
    auto conf = mock_conf.get_settings();
    std::cout << "parsing from config - ";
    CHECK_EXPR( conf.pattern_shift.size() == 3 && !conf.root_dir.empty() && conf.threads == 4, 
                "incorrect parsing (depends on the config file, may not be correct)");

    mock_config mock_conf2;
    std::string root = "root_dir";
    std::string root_path = "qweasd";
    std::string th_amount = "threads";
    std::string correct_pattern = "<%PATTERN%>";
    std::string bad_pattern1 = "<incorrect>";
    std::string bad_pattern2 = "<incorrect%>";

    mock_conf2.add_setting(root, root_path);
    mock_conf2.add_setting(th_amount, "5");
    mock_conf2.add_setting(correct_pattern, "q");

    std::cout   << "incorrect pattern 1 - "
                << CHECK_THROWS([&mock_conf2, &bad_pattern1](){mock_conf2.add_setting(bad_pattern1, "w");}) << std::endl;
    std::cout   << "incorrect pattern 2 - "
                << CHECK_THROWS([&mock_conf2, &bad_pattern2](){mock_conf2.add_setting(bad_pattern2, "e");}) << std::endl;
    std::cout << "add setting - ";

    CHECK_EXPR( mock_conf2.get_settings().pattern_shift.size() == 1 &&
                mock_conf2.get_settings().root_dir == root_path &&
                mock_conf2.get_settings().threads == 5, 
                "incorrect add setting");

    std::cout << "----------------------" << std::endl;
}

void parser_tests()
{
    std::cout << "--------ParserTests--------" << std::endl;
    std::string correct_path = std::filesystem::current_path().string() + "/config.txt";
    mock_config mock_conf(correct_path);
    app::core::thread_pool th_pool(mock_conf);

    mock_parser mock_p(mock_conf, th_pool);
    std::cout << "recursive files parsing 1 - ";
    CHECK_EXPR(mock_p.get_files().size() == 5, "incorrect (depends on input, may not be correct)");

    std::string root = "root_dir";
    std::string root_path = "qweasd";
    mock_conf.add_setting(root, root_path);
    std::cout   << "recursive files parsing 2 - "
                << CHECK_THROWS([&mock_conf, &th_pool](){mock_parser mock_p2(mock_conf, th_pool);}) << std::endl;

    std::cout << "----------------------" << std::endl;
}

int main()
{
    config_tests(); 
    parser_tests();
    //sry i can write more and best tests but, without catch or smth else that x10 hell
    //im a little sorry to waste my time on this, do not judge strictly :)
    return EXIT_SUCCESS;
}