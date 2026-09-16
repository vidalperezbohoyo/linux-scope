#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <functional>

#include "Utility/Thread.h"
#include "Utility/Singleton.h"
#include "Utility/Log.h"

class AutoExposure : public Thread, public Singleton<AutoExposure>
{
private:
    const uint8_t FRAMES_BETWEEN_ITERATIONS = 10; // How many frames between adjustements. Range: 1(fast adjustment-High CPU) to 255(slow adjustment-LOW CPU)

public:
    friend class Singleton<AutoExposure>;

    ~AutoExposure();
    
    void loop() override;

    void devour(const cv::Mat& frame);

private:
    AutoExposure();

    void set(const int& exposure, const int& gain);

private:
    uint8_t frame_count_ = 0;

    int v4l2_control_fd_ = -1;
};