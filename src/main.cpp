#include <iostream>

#include "VideoProvider.h"
#include "VideoProcessor.h"

// Select display type
#define USE_OPENCV_DISPLAY 1
#define USE_FB_DISPLAY 0

#if USE_OPENCV_DISPLAY
    #include "CvDisplay.h"
#elif USE_FB_DISPLAY
    #include "FbDisplay.h"
#endif


int main() 
{

    VideoProcessor processor;

#if USE_OPENCV_DISPLAY
    CvDisplay display;
#elif USE_FB_DISPLAY
    FbDisplay display;
#endif

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