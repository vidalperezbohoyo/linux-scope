#include "Camera/AutoExposure.h"

AutoExposure::AutoExposure()
{
}

AutoExposure::~AutoExposure()
{
    close(v4l2_control_fd_);
}

bool AutoExposure::init()
{
    int v4l2_control_fd_ = open("/dev/v4l-subdev2", O_RDWR);

    if (v4l2_control_fd_ < 0)
    {
        Log::instance().error("[AutoExposure::AutoExposure] Device: /dev/v4l-subdev2 not oppened");
        return false;
    }
    return true;
}

void AutoExposure::loop()
{   
    // Sleep until the devour() method sets the wake_up flag to true
    std::unique_lock<std::mutex> lock(mtx_);
    cv_.wait(lock, [this] { return wake_up_;});
    wake_up_ = false;

    // Extract percentiles from the luminance channel
    std::vector<uint8_t> luminance_values;
    luminance_values.reserve(luminance_mat_.total());
    for (int i = 0; i < luminance_mat_.rows; ++i)
    {
        for (int j = 0; j < luminance_mat_.cols; ++j)
        {
            luminance_values.push_back(luminance_mat_.at<uint8_t>(i, j));
        }
    }
    std::sort(luminance_values.begin(), luminance_values.end());
    uint8_t lower_percentile = luminance_values[static_cast<size_t>(0.1 * luminance_values.size())];
    uint8_t upper_percentile = luminance_values[static_cast<size_t>(0.9 * luminance_values.size())];

    Log::instance().debug("[AutoExposure::loop] Lower Percentile: {0}, Upper Percentile: {1}", lower_percentile, upper_percentile);
}

void AutoExposure::devour(void* raw_nv12)
{
    frame_count_++;
    if (frame_count_ >= VIDEO_AUTO_EXPOSURE_FRAMES_BETWEEN_ITERATIONS)
    {
        frame_count_ = 0;

        // Extract luminance channel from NV12 frame and copy to object memory
        luminance_mat_ = cv::Mat(480, 640, CV_8UC1, static_cast<uint8_t*>(raw_nv12)).clone();

        // Activate thread flag to perform auto exposure adjustment in the next loop iteration
        {
            std::lock_guard<std::mutex> lock(mtx_); // Wait for the lock to be acquired before setting the flag
            wake_up_ = true;
        }

        cv_.notify_one(); // Wake up the thread to perform auto exposure adjustment
    }
}

void AutoExposure::set(const int& exposure, const int& gain)
{
    struct v4l2_control ctrl{};

    ctrl.id = V4L2_CID_EXPOSURE;
    ctrl.value = exposure;

    ctrl.id = V4L2_CID_GAIN;
    ctrl.value = gain;

    ioctl(v4l2_control_fd_, VIDIOC_S_CTRL, &ctrl);
}
