#pragma once

#include "VideoProcessor.h"

class HUD
{

public:
    HUD();
    ~HUD();

    void draw(cv::Mat& in_240p, cv::Mat& out_240p);

private:
    cv::Scalar crosshair_color = cv::Scalar(0, 255, 0); // Color of the crosshair in BGR format

    uint8_t magnification = 1; // Magnification of the scope. E.g., 1x, 2x, 4x, etc.
    uint8_t distance = 10; // Distance to the target in meters
};