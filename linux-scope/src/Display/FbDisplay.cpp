#include "Display/FbDisplay.h"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <unistd.h>

#include <cstring>

bool FbDisplay::init()
{
    fb_fd_ = open("/dev/fb0", O_RDWR);

    if (fb_fd_ < 0)
        return false;

    ioctl(fb_fd_, FBIOGET_FSCREENINFO, &finfo_);
    ioctl(fb_fd_, FBIOGET_VSCREENINFO, &vinfo_);

    framebuffer_size_ = finfo_.smem_len;

    framebuffer_ = static_cast<uint8_t*>(
        mmap(nullptr,
             framebuffer_size_,
             PROT_READ | PROT_WRITE,
             MAP_SHARED,
             fb_fd_,
             0));

    if (framebuffer_ == MAP_FAILED)
    {
        framebuffer_ = nullptr;
        close(fb_fd_);
        return false;
    }

    return true;
}

FbDisplay::~FbDisplay()
{
    if (framebuffer_)
        munmap(framebuffer_, framebuffer_size_);

    if (fb_fd_ >= 0)
        close(fb_fd_);
}

void FbDisplay::draw(const cv::Mat& frame)
{
    if (!framebuffer_)
        return;

    auto t0 = std::chrono::steady_clock::now();
    
    static cv::Mat resized;

    if (frame.cols != static_cast<int>(vinfo_.xres) ||
        frame.rows != static_cast<int>(vinfo_.yres))
    {
        // cv::resize(
        //     frame,
        //     resized,
        //     cv::Size(vinfo_.xres, vinfo_.yres),
        //     0,
        //     0,
        //     cv::INTER_NEAREST);

        // Crop to 240x240
        int crop_size = 240;
        int x_offset = (frame.cols - crop_size) / 2;
        int y_offset = (frame.rows - crop_size) / 2;
        cv::Rect roi(x_offset, y_offset, crop_size, crop_size);
        resized = frame(roi);
    }
    else
    {
        resized = frame;
    }
    auto t1 = std::chrono::steady_clock::now();

    static cv::Mat rgb565;
    cv::cvtColor(resized, rgb565, cv::COLOR_BGR2BGR565);

    auto t2 = std::chrono::steady_clock::now();
    const size_t bytes =
        rgb565.cols * rgb565.rows * 2;

    memcpy(framebuffer_, rgb565.data, bytes);

    auto t3 = std::chrono::steady_clock::now();

    std::cout
    << "resize: "
    << std::chrono::duration<double,std::milli>(t1-t0).count()
    << " ms\n";

    std::cout
        << "cvtColor: "
        << std::chrono::duration<double,std::milli>(t2-t1).count()
        << " ms\n";

    std::cout
        << "memcpy: "
        << std::chrono::duration<double,std::milli>(t3-t2).count()
        << " ms\n";
}

void FbDisplay::clear()
{
    memset(framebuffer_, 0, framebuffer_size_);
}