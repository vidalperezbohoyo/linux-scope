#pragma once

#if defined(BUILD_X86)
#include "Display/CvDisplay.h"
#elif defined(BUILD_LUCKFOX_PICO)
#include "Display/FbDisplay.h"
#endif

#include <opencv2/opencv.hpp>
#include <mutex>
#include <condition_variable>

#if defined(BUILD_X86)
#include "Display/CvDisplay.h"
#elif defined(BUILD_LUCKFOX_PICO)
#include "Display/FbDisplay.h"
#endif 

#include "Utility/Thread.h"
#include "Utility/Singleton.h"
#include "Utility/Log.h"

class DisplayManager : public Thread, public Singleton<DisplayManager>
{
public:
    friend class Singleton<DisplayManager>;

    void loop() override;

    bool init();

    void setCameraImage(const cv::Mat& image);

private:


private:
    cv::Mat latest_camera_image_;
    bool latest_camera_image_ready_ = false;
    std::condition_variable camera_image_cv;
    std::mutex camera_image_mutex_;
};


