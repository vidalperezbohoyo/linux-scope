#pragma once

extern "C" {
    #include "fbgraphics.h"
    #include "fbg_fbdev.h"
}

#include "BaseDisplay.h"

class FbDisplay : public BaseDisplay
{
public:    
    bool init() override;
    
    void draw(const cv::Mat& frame) override;

    void clear() override;

private:
    struct _fbg *fbg_;
};