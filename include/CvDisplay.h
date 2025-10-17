#pragma once
#include "BaseDisplay.h"

class FbDisplay : public BaseDisplay
{

public:
    FbDisplay();
    ~FbDisplay();
    
    bool init() override;
    
    void draw(const cv::Mat& frame) override;

    void clear();
};