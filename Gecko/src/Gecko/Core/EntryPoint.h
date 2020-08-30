#pragma once
#include "Application.h"

int main(int argc, char const *argv[])
{
    auto application = Gecko::CreateApplication();
    
    try
    {
        application->Run();
    }
    catch (const std::exception &e)
    {
        GK_LOG(GK_ERROR) << e.what() << '\n';
    }

    return 0;
} 

