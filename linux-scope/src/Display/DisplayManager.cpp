#include "Display/DisplayManager.h"

bool DisplayManager::init()
{
    #if defined(BUILD_X86)
        BaseDisplay& display = CvDisplay::instance();
    #elif defined(BUILD_LUCKFOX_PICO)
        BaseDisplay& display = FbDisplay::instance();
    #endif

    return display.init();
}


void DisplayManager::setCameraImage(const cv::Mat& image)
{
    {
        std::unique_lock<std::mutex> lock(camera_image_mutex_, std::try_to_lock);

        if (!lock.owns_lock())
        {
            //Log::instance().warning("[DisplayManager::setCameraImage] Could not acquire lock, previous image processing is still ongoing, skipping this frame.");
            return; // Bad thing, optimization: skip this frame, processing is too long
        }

        latest_camera_image_ = image.clone(); // Copy buffer
        latest_camera_image_ready_ = true; // Condition flag
    }

    camera_image_cv.notify_one();
    // This function runs in VideoProvider Thread, so no more code here.
}

void DisplayManager::loop()
{
    // Sleep thread until new image is ready.
    std::unique_lock<std::mutex> lock(camera_image_mutex_); // Unique lock is for condition variable
   
    camera_image_cv.wait(lock, [this] { return latest_camera_image_ready_; });
    
    // The condition variable unlocks the mutex and sleep until condition is met, then locks mutex again
    // Now the mutex is locked in whole processing, so no more image can be set until processing is done.

    // Perform image processing

    Log::instance().debug("[DisplayManager::loop] Received new camera image, processing...");

    // 1. Image is on NV12 format, convert to BGR
    cv::cvtColor(latest_camera_image_, latest_camera_image_, cv::COLOR_YUV2BGR_NV12);
    // 2. Show
    FbDisplay::instance().draw(latest_camera_image_);
    latest_camera_image_ready_ = false;



}