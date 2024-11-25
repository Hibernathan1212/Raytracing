#include <iostream>
#include "Application.hpp"

bool g_ApplicationRunning = true;

int main(int argc, char * argv[]) {
    while (g_ApplicationRunning)
    {
        Application* app = CreateApplication(argc, argv);
        app->Run();
        delete app;
    }
    return 0;
}

/*
 g_ = global variable
 m_ = member variable
 u_ = variable for uniform buffer
 */
