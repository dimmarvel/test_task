#include "app.hpp"
#include <iostream>
#include <filesystem>

int main(int argc, char *argv[])
{
    try
    {
        std::string path;
        if(argc > 1)
            path = argv[1];
        else
            path = std::filesystem::current_path().string() + "/config.txt";
        
        app::application app(path);
        app.start();
    }
    catch(const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
