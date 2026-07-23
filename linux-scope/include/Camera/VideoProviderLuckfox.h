#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <functional>

#include "Utility/Thread.h"
#include "Utility/Singleton.h"
#include "Utility/Log.h"
#include "Camera/V4L2Capture.h"

class VideoProvider : public Thread, public Singleton<VideoProvider>
{
public:
    friend class Singleton<VideoProvider>;

    ~VideoProvider();
    
    bool init();

    void loop() override;

    void setImageCallback(std::function<void(const cv::Mat&)> callback);

private:
    VideoProvider();

private:
    V4L2Capture cap_;

    std::function<void(const cv::Mat&)> image_callback_;
};