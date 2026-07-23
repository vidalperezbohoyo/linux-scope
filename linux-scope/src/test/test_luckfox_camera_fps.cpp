#if defined(BUILD_LUCKFOX_PICO)
#include <unistd.h>
#include <chrono>

#include "Camera/VideoProvider.h"
#include "Utility/Log.h"

int main() 
{
    Log::instance().set_level(Log::Level::Debug);

    auto& video_provider = VideoProvider::instance();

    if (!video_provider.init()) 
    {
        Log::instance().error("Failed to initialize video provider.");
        return -1;
    }

    video_provider.start();

    video_provider.setImageCallback(
        [last = std::chrono::steady_clock::now()](const cv::Mat& mat) mutable
        {
            if (mat.empty()) 
            {
                Log::instance().error("Received empty frame.");
                exit(-1);
            }

            auto now = std::chrono::steady_clock::now();

            double dt = std::chrono::duration<double>(now - last).count();
            last = now;

            double fps = 1.0 / dt;

            Log::instance().debug(
                "dt = " + std::to_string(dt * 1000.0) +
                " ms, fps = " + std::to_string(fps));
        });

    while (true) 
    {
        // Keep the application running to receive frames
        usleep(100000); // Sleep for 100ms
    }

    return 0;
}
























#endif

