#include <iostream>

#include "VideoProvider.h"
#include "VideoProcessor.h"

#ifdef USE_CV_DISPLAY
    #include "CvDisplay.h"
#endif

#ifdef USE_FB_DISPLAY
    #include "FbDisplay.h"
#endif


int main() 
{
#ifdef USE_CV_DISPLAY
    CvDisplay display;
#endif

#ifdef USE_FB_DISPLAY
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

    processor.start();

    while(true);

    return 0;
}