#include "Camera/V4L2Capture.h"

V4L2Capture::V4L2Capture() : fd_(-1)
{
    Log::instance().info("V4L2Capture initialized");
}

V4L2Capture::~V4L2Capture()
{
    release();
}

void V4L2Capture::release()
{
    if (fd_ >= 0)
    {
        int type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        ioctl(fd_, VIDIOC_STREAMOFF, &type);

        for (auto& buffer : buffers_)
        {
            munmap(buffer.start, buffer.length);
        }

        ::close(fd_);
        fd_ = -1;
    }
}

void V4L2Capture::open(const std::string& device, int width, int height)
{
    fd_ = ::open(device.c_str(), O_RDWR);
    if (fd_ < 0)
    {
        Log::instance().error("Failed to open device: {0}", device);
        return;
    }

    v4l2_format fmt{};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    fmt.fmt.pix_mp.width = width;
    fmt.fmt.pix_mp.height = height;
    fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12; // Using NV12 format
    fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;

    if (ioctl(fd_, VIDIOC_S_FMT, &fmt) < 0)
    {
        Log::instance().error("Failed to set format on device: {0}", device);
        release();
        return;
    }

    // Request buffers
    v4l2_requestbuffers req{};
    req.count = 4; // Number of buffers
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    req.memory = V4L2_MEMORY_MMAP;

    if (ioctl(fd_, VIDIOC_REQBUFS, &req) < 0)
    {
        Log::instance().error("Failed to request buffers on device: {0}", device);
        release();
        return;
    }

    buffers_.resize(req.count);

    // Map buffers
    for(unsigned i=0;i<req.count;i++)
    {
        v4l2_buffer buf{};
        v4l2_plane planes[VIDEO_MAX_PLANES]{};

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        buf.length = 1;
        buf.m.planes = planes;

        ioctl(fd_, VIDIOC_QUERYBUF, &buf);

        buffers_[i].length = planes[0].length;

        buffers_[i].start =
            mmap(nullptr,
                planes[0].length,
                PROT_READ|PROT_WRITE,
                MAP_SHARED,
                fd_,
                planes[0].m.mem_offset);
    }
    // Queue buffers
    for(unsigned i=0;i<req.count;i++)
    {
        v4l2_buffer buf{};
        v4l2_plane planes[VIDEO_MAX_PLANES]{};

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        buf.length = 1;
        buf.m.planes = planes;

        ioctl(fd_, VIDIOC_QBUF, &buf);
    }

    // Start streaming
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

    ioctl(fd_, VIDIOC_STREAMON, &type);

}

bool V4L2Capture::read(cv::Mat& image)
{

    if (fd_ < 0)
    {
        throw std::runtime_error("Device not opened");
    }

    v4l2_buffer buf{};
    v4l2_plane planes[VIDEO_MAX_PLANES]{};

    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.length = 1;
    buf.m.planes = planes;

    // Wait for a frame
    if (ioctl(fd_, VIDIOC_DQBUF, &buf) < 0)
    {
        Log::instance().error("[V4L2Capture::read] Failed to dequeue the buffer");
        return false;
    }

    void* raw = buffers_[buf.index].start;

    // Fast clone
    const int rows = 480 + 480 / 2; // 720
    const int cols = 640;
    const size_t data_size = rows * cols * sizeof(uint8_t);

    image.create(rows, cols, CV_8UC1);
    std::memcpy(image.data, raw, data_size);

    // Requeue the buffer
    if (ioctl(fd_, VIDIOC_QBUF, &buf) < 0)
    {
        Log::instance().error("[V4L2Capture::read] Failed to requeue the buffer");
        return false;
    }

    // Debug
    auto now = std::chrono::steady_clock::now();

    double dt = std::chrono::duration<double>(now - last_frame_time_).count();
    last_frame_time_ = now;

    double fps = 1.0 / dt;

    Log::instance().debug(
        "V4L2Capture: dt = " + std::to_string(dt * 1000.0) +
        " ms, fps = " + std::to_string(fps));

    std::cout << std::endl;

    return true;
}