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

    cv::Mat resized;

    if (frame.cols != static_cast<int>(vinfo_.xres) ||
        frame.rows != static_cast<int>(vinfo_.yres))
    {
        cv::resize(
            frame,
            resized,
            cv::Size(vinfo_.xres, vinfo_.yres),
            0,
            0,
            cv::INTER_LINEAR);
    }
    else
    {
        resized = frame;
    }

    cv::Mat rgb565;
    cv::cvtColor(resized, rgb565, cv::COLOR_BGR2BGR565);

    const size_t bytes =
        rgb565.cols * rgb565.rows * 2;

    memcpy(framebuffer_, rgb565.data, bytes);
}

void FbDisplay::clear()
{
    memset(framebuffer_, 0, framebuffer_size_);
}