#include "Camera/AutoExposure.h"

AutoExposure::AutoExposure()
{
    int fd = open("/dev/v4l-subdev2", O_RDWR);

    if (v4l2_control_fd_ < 0)
    {
        Log::error("[AutoExposure::AutoExposure] Device: /dev/v4l-subdev2 not oppened")
    }

}

AutoExposure::~AutoExposure()
{
}

void AutoExposure::loop()
{
    // Wait flag
    // Perform
    // Sleep
}

void AutoExposure::devour(const cv::Mat& frame)
{
    frame_count_++;
    if (frame_count_ == FRAMES_BETWEEN_ITERATIONS)
    {
        frame_count_ = 0;

        // Copy frame to memory (a bit delay)
        // Wake Up Thread with flag (instantaneous)
    }
}

void AutoExposure::set(const int& exposure, const int& gain)
{

    struct v4l2_control ctrl{};

    ctrl.id = V4L2_CID_EXPOSURE;
    ctrl.value = 100;

    ioctl(fd, VIDIOC_SUBDEV_S_CTRL, &ctrl);

    close(fd);

    v4l2_subdev_set_control(EXPOSURE, ae.exposure);
    v4l2_subdev_set_control(GAIN, ae.gain);
}
