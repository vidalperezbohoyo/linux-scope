#if defined(BUILD_LUCKFOX_PICO)

#include <unistd.h>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <iostream>

#include "Camera/VideoProviderLuckfox.h"
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

    cv::Mat last_frame;
    std::mutex frame_mutex;
    std::condition_variable frame_cv;

    video_provider.start();

    video_provider.setImageCallback(
        [&display, &last_frame, &frame_mutex, &frame_cv](const cv::Mat& frame) 
        {

            auto start = std::chrono::steady_clock::now();

            //display.draw(frame);
            usleep(10000); // Sleep for 10ms

            auto end = std::chrono::steady_clock::now();

            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            std::cout << "draw() tardó " << ms.count() << " ms\n";;

            // {
            //     std::lock_guard<std::mutex> lock(frame_mutex);
            //     last_frame = frame.clone();
            // }

            // frame_cv.notify_one();
        });

    while (true) 
    {
        // cv::Mat frame_to_draw;
        // {
        //     std::unique_lock<std::mutex> lock(frame_mutex);
        //     frame_cv.wait(lock, [&last_frame] { return !last_frame.empty(); });

        //     frame_to_draw = last_frame.clone();
        // }

        // //display.draw(frame_to_draw);

        // // Clear the last frame after drawing
        // last_frame.release();

        usleep(100000); // Sleep for 100ms
    }

    return 0;
}
























#endif

