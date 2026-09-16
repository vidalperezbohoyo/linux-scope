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
        cv::flip(frame, frame, 1);
        image_callback_(frame);
    }
    else
    {
        Log::instance().error("aaaaaaaaaFailed to capture frame or image callback not set.");
        this->stop();
        exit(-1);
    }
}

bool VideoProvider::init()
{
    Log::instance().info("Initializing VideoProvider...");
    cap_.release();
    cap_.open(0, cv::CAP_V4L2);
    cap_.set(cv::CAP_PROP_FOURCC,
            cv::VideoWriter::fourcc('M','J','P','G'));
    cap_.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap_.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap_.set(cv::CAP_PROP_FPS, 30);
    return cap_.isOpened();
}

void VideoProvider::setImageCallback(std::function<void(const cv::Mat&)> callback)
{
    image_callback_ = callback;
}


