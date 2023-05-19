#include "app.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        app::application app("/home/dmatsiukhov/git_repos/test_task/config.txt"); // TODO: change hardcode to argc argv
        app.start();
    }
    catch(const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << '\n';
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
