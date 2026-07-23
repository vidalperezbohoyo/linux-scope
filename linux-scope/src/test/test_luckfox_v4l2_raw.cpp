#if defined(BUILD_LUCKFOX_PICO)
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

#include "Display/FbDisplay.h"

#include "Utility/Log.h"

int main() 
{
    Log::instance().set_level(Log::Level::Debug);

    auto& display = FbDisplay::instance();

    if (!display.init()) 
    {
        Log::instance().error("Failed to initialize display.");
        return -1;
    }

    int fd = open("/dev/video11", O_RDWR);

    if (fd < 0)
    {
        Log::instance().error("Failed to open /dev/video11");
        return -1;
    }

    // Multiplanar
    v4l2_format fmt{};

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

    fmt.fmt.pix_mp.width  = 640;
    fmt.fmt.pix_mp.height = 480;
    fmt.fmt.pix_mp.pixelformat = V4L2_PIX_FMT_NV12;
    fmt.fmt.pix_mp.field = V4L2_FIELD_NONE;

    ioctl(fd, VIDIOC_S_FMT, &fmt);

    std::cout << "bytesperline = "
            << fmt.fmt.pix_mp.plane_fmt[0].bytesperline
            << std::endl;

    std::cout << "sizeimage = "
            << fmt.fmt.pix_mp.plane_fmt[0].sizeimage
            << std::endl;

    printf("bytesperline=%d\n",
       fmt.fmt.pix_mp.plane_fmt[0].bytesperline);

    printf("sizeimage=%d\n",
        fmt.fmt.pix_mp.plane_fmt[0].sizeimage);

    // Request buffers
    v4l2_requestbuffers req{};

    req.count = 4;
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
    req.memory = V4L2_MEMORY_MMAP;

    ioctl(fd, VIDIOC_REQBUFS, &req);

    // Map buffers
    struct Buffer
    {
        void* start;
        size_t length;
    };

    std::vector<Buffer> buffers(req.count);

    for(unsigned i=0;i<req.count;i++)
    {
        v4l2_buffer buf{};
        v4l2_plane planes[VIDEO_MAX_PLANES]{};

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        buf.length = 1;
        buf.m.planes = planes;

        ioctl(fd, VIDIOC_QUERYBUF, &buf);

        buffers[i].length = planes[0].length;

        buffers[i].start =
            mmap(nullptr,
                planes[0].length,
                PROT_READ|PROT_WRITE,
                MAP_SHARED,
                fd,
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

        ioctl(fd, VIDIOC_QBUF, &buf);
    }

    // Start streaming
    int type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;

    ioctl(fd, VIDIOC_STREAMON, &type);

    // Frames
    while (true)
    {
        v4l2_buffer buf{};
        v4l2_plane planes[VIDEO_MAX_PLANES]{};

        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.length = 1;
        buf.m.planes = planes;

        // Esperar un frame
        if (ioctl(fd, VIDIOC_DQBUF, &buf) < 0)
        {
            perror("VIDIOC_DQBUF");
            break;
        }

        void* raw = buffers[buf.index].start;

        // NV12 -> OpenCV
        cv::Mat nv12(
            480 + 480 / 2,
            640,
            CV_8UC1,
            raw);

        cv::Mat bgr;
        cv::cvtColor(nv12, bgr, cv::COLOR_YUV2BGR_NV12);

        cv::Mat scaled_frame;
        cv::resize(bgr, scaled_frame, cv::Size(240, 240));

        display.draw(scaled_frame);

        // Requeue the buffer
        if (ioctl(fd, VIDIOC_QBUF, &buf) < 0)
        {
            perror("VIDIOC_QBUF");
            break;
        }
    }
        return 0;
}
























#endif

