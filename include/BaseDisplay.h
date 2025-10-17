#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class BaseDisplay
{
public:
    virtual ~BaseDisplay() = default;

    virtual bool init() = 0;
    
    virtual void draw(const cv::Mat& frame) = 0;

    virtual void clear() = 0;
};