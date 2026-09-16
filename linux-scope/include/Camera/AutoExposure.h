#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#include "Utility/Thread.h"
#include "Utility/Singleton.h"
#include "Utility/Log.h"
#include "Utility/Defines.h"

class AutoExposure : public Thread, public Singleton<AutoExposure>
{
public:
    friend class Singleton<AutoExposure>;

    ~AutoExposure();
    
    void loop() override;

    bool init();

    void devour(void* raw_nv12);

private:
    AutoExposure();

    void set(const int& exposure, const int& gain);

private:
    uint8_t frame_count_ = 0;

    int v4l2_control_fd_ = -1;

    cv::Mat luminance_mat_;

    std::mutex mtx_;
    std::condition_variable cv_;
    bool wake_up_ = false;
};