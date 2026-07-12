#pragma once

#if defined(BUILD_X86)
    #include <opencv2/opencv.hpp>
#elif defined(BUILD_LUCKFOX_PICO)
    #include <opencv2/core/core.hpp>
    #include <opencv2/imgproc/imgproc.hpp>
#endif

class BaseDisplay
{
public:
    virtual ~BaseDisplay() = default;

    virtual bool init() = 0;
    
    virtual void draw(const cv::Mat& frame) = 0;

    virtual void clear() = 0;
};