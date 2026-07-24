#include "Camera/VideoProviderLuckfox.h"

VideoProvider::VideoProvider()
{
}

VideoProvider::~VideoProvider()
{
    cap_.release();
}

void VideoProvider::loop()
{   
    static cv::Mat frame;

    bool ret = cap_.read(frame);
   
    if (ret && image_callback_)
    {
        image_callback_(frame); // As reference
    }
    else
    {
        Log::instance().error("Failed to capture frame or image callback not set.");
    }
}

bool VideoProvider::init()
{
    Log::instance().info("Initializing VideoProvider...");
    cap_.release();
    cap_.open("/dev/video11", 640, 480);
    return cap_.isOpened();
}

void VideoProvider::setImageCallback(std::function<void(const cv::Mat&)> callback)
{
    image_callback_ = callback;
}


