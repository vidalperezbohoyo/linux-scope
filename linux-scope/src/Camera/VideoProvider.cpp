#include "Camera/VideoProvider.h"

VideoProvider::VideoProvider()
{
}

VideoProvider::~VideoProvider()
{
    cap_.release();
}

void VideoProvider::loop()
{   
    cv::Mat frame;
    cap_ >> frame;
   
    onImage(frame);
}

bool VideoProvider::init()
{
    cap_.release();
    cap_.open(0);
    return cap_.isOpened();
}
