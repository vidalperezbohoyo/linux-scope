#pragma once
#include "BaseDisplay.h"

class CvDisplay : public BaseDisplay
{

public:
    CvDisplay();
    ~CvDisplay();
    
    bool init() override;
    
    void draw(const cv::Mat& frame) override;

    void clear();
};