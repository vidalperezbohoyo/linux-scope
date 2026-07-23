#pragma once

#include <linux/fb.h>

#include "Display/BaseDisplay.h"
#include "Utility/Singleton.h"

// Debug
#include <chrono>
#include <iostream>

class FbDisplay : public BaseDisplay, public Singleton<FbDisplay>
{
public:
    friend class Singleton<FbDisplay>;

    ~FbDisplay();

    bool init() override;
    
    void draw(const cv::Mat& frame) override;

    void clear() override;

private:
    int fb_fd_ = -1;

    fb_fix_screeninfo finfo_{};
    fb_var_screeninfo vinfo_{};

    uint8_t* framebuffer_ = nullptr;
    size_t framebuffer_size_ = 0;
};