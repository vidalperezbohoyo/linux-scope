#pragma once

#include "Utility/Singleton.h"
#include "Display/BaseDisplay.h"

#if defined(BUILD_X86)
class CvDisplay : public BaseDisplay, public Singleton<CvDisplay>
{
public:
    friend class Singleton<CvDisplay>;

    ~CvDisplay();

    bool init() override;
    
    void draw(const cv::Mat& frame) override;

    void clear();

private:
    CvDisplay();
};
#endif