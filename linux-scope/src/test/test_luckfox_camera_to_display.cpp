#if defined(BUILD_LUCKFOX_PICO)

#include <unistd.h>

#include "Camera/VideoProvider.h"
#include "Display/FbDisplay.h"
#include "Utility/Log.h"

int main() 
{
    Log::instance().set_level(Log::Level::Debug);

    auto& video_provider = VideoProvider::instance();
    auto& display = FbDisplay::instance();

    if (!display.init()) 
    {
        Log::instance().error("Failed to initialize display.");
        return -1;
    }

    if (!video_provider.init()) 
    {
        Log::instance().error("Failed to initialize video provider.");
        return -1;
    }

    video_provider.setImageCallback(
        [&display](const cv::Mat& frame) 
        {
            // Scale frame to 240x240 for display
            cv::Mat scaled_frame;
            cv::resize(frame, scaled_frame, cv::Size(240, 240));
            display.draw(scaled_frame);
        });

    while (true) 
    {
        // Keep the application running to receive frames
        usleep(100000); // Sleep for 100ms
    }

    return 0;
}
























#endif

