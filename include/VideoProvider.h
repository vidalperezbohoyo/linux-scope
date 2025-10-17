#pragma once
#include <opencv2/opencv.hpp>
#include <string>

#include "Thread.h"

class VideoProvider : public Thread
{
public:
    VideoProvider();
    ~VideoProvider();
    
    bool init();

    void loop() override;

    virtual void onImage(const cv::Mat& frame) = 0;
    
private:
    cv::VideoCapture cap_;
};