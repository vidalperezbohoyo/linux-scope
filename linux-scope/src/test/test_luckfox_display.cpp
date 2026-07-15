#if defined(BUILD_LUCKFOX_PICO)
#include <unistd.h>

#include "Display/FbDisplay.h"
#include "Utility/Log.h"

int main() 
{
    Log::instance().set_level(Log::Level::Debug);

    auto& display = FbDisplay::instance();

    if (!display.init()) 
    {
        Log::instance().error("Failed to initialize display.");
        return -1;
    }

    // Example draw: Red (with text that put Red), Green (with text that put Green), Blue (with text that put Blue)
    cv::Mat red_screen(240, 240, CV_8UC3, cv::Scalar(0, 0, 255));
    cv::putText(red_screen, "Red", cv::Point(60, 120), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

    cv::Mat green_screen(240, 240, CV_8UC3, cv::Scalar(0, 255, 0));
    cv::putText(green_screen, "Green", cv::Point(40, 120), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

    cv::Mat blue_screen(240, 240, CV_8UC3, cv::Scalar(255, 0, 0));
    cv::putText(blue_screen, "Blue", cv::Point(60, 120), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2);

    while(1)
    {
        display.draw(red_screen);
        usleep(1000000); // Wait for 1 second
        display.draw(green_screen);
        usleep(1000000); // Wait for 1 second
        display.draw(blue_screen);
        usleep(1000000); // Wait for 1 second
    }

    return 0;
}
























#endif

