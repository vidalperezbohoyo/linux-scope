#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include <sys/types.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>

#include "Utility/Log.h"

class V4L2Capture
{
public:

    V4L2Capture();
    ~V4L2Capture();

    void open(const std::string& device, int width, int height);
    void release();

    bool read(cv::Mat& image);

    bool isOpened() const { return fd_ >= 0; }

private:
    struct Buffer
    {
        void* start;
        size_t length;
    };

    int fd_;
    std::vector<Buffer> buffers_;

    // Debug
    std::chrono::steady_clock::time_point last_frame_time_ = std::chrono::steady_clock::now();
};