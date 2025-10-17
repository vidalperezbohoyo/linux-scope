#include "VideoProvider.h"

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
    bool ret = cap_.read(frame);
    if (ret && !frame.empty())
    {
        onImage(frame);
    }
    else
    {
        std::cerr << "Failed to read frame from video source." << std::endl;
    }
}

bool VideoProvider::init()
{
    cap.release();
    cap_.open("/dev/video0");
    return m_capture.isOpened();
}
