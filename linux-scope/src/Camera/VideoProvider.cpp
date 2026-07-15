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
   
    if (!frame.empty() && image_callback_)
    {
        image_callback_(frame);
    }
}

bool VideoProvider::init()
{
    cap_.release();
    cap_.open(0);
    return cap_.isOpened();
}

void VideoProvider::setImageCallback(std::function<void(const cv::Mat&)> callback)
{
    image_callback_ = callback;
}


