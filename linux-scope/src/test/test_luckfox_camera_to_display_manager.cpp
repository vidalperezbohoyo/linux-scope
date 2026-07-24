#if defined(BUILD_LUCKFOX_PICO)

#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>

#include "Camera/VideoProviderLuckfox.h"
#include "Display/DisplayManager.h"
#include "Utility/Log.h"

int main() 
{
    Log::instance().set_level(Log::Level::Debug);

    auto& video_provider = VideoProvider::instance();
    auto& display_manager = DisplayManager::instance();
   
    if (!video_provider.init()) 
    {
        Log::instance().error("Failed to initialize video provider.");
        return -1;
    }

    if (!display_manager.init()) 
    {
        Log::instance().error("Failed to initialize video provider.");
        return -1;
    }

    video_provider.start();
    display_manager.start();

    video_provider.setImageCallback([&display_manager](const cv::Mat& image) {
        display_manager.setCameraImage(image);
    });
    
    while(1)
    {
        usleep(100000); // Sleep for 100ms
    }

    return 0;
}
























#endif

