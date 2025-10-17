#pragma once

extern "C" {
    #include "fbgraphics.h"
    #include "fbg_fbdev.h"
}

#include "BaseDisplay.h"

class FbDisplay : public BaseDisplay
{
private:
    const std::string DEVICE_PATH = "/dev/fb0";
    
public:    
    bool init() override;
    
    void draw(const cv::Mat& frame) override;

    void clear() override;

private:
    struct _fbg *fbg_;
};