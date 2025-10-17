#include <iostream>

#include "VideoProvider.h"
#include "VideoProcessor.h"

// Select display type
#define USE_OPENCV_DISPLAY 0
#define USE_FB_DISPLAY 1

#if USE_OPENCV_DISPLAY
    #include "CvDisplay.h"
#elif USE_FB_DISPLAY
    #include "FbDisplay.h"
#endif


int main() 
{
#if USE_OPENCV_DISPLAY
    CvDisplay display;
#elif USE_FB_DISPLAY
    FbDisplay display;
#endif

    if (!display.init()) 
    {
        std::cerr << "Failed to initialize display." << std::endl;
        return -1;
    }

    VideoProcessor processor;
    if (!processor.init()) 
    {
        std::cerr << "Failed to initialize video processor." << std::endl;
        return -1;
    }

    processor.setImageCallback([&display](const cv::Mat& frame) 
    {
        display.draw(frame);
    });

    while(true);

    return 0;
}