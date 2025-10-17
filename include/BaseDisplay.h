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

protected:
    uint16_t getWidth() const;
    uint16_t getHeight() const;
    
private:
    uint16_t width_;
    uint16_t height_;
};