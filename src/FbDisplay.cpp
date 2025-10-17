#include "FbDisplay.h"

bool FbDisplay::init()
{
   fbg_ = fbg_fbdevSetup((char*)DEVICE_PATH.c_str(), 0); 
   return fbg_ != nullptr; 
}

void FbDisplay::draw(const cv::Mat& frame)
{
    uint8_t* buffer = nullptr;

    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(fbg_->width, fbg_->height));
    cv::cvtColor(resized, resized, cv::COLOR_BGR2RGB);

    // Convert to PNG
    std::vector<uint8_t> png_vec;
    if (!cv::imencode(".png", resized, png_vec))
    {
        std::cerr << "[ERROR] encoding image to PNG" << std::endl;
        return;
    }
    size_t buf_size = png_vec.size();
    buffer = (uint8_t*) malloc(buf_size);
    if (!buffer) 
    {
        std::cerr << "[ERROR] Malloc() error" << std::endl;
        return;
    }
    std::memcpy(buffer, png_vec.data(), buf_size);

    // Load image into FBG
    struct _fbg_img* img = fbg_loadImageFromMemory(fbg_, buffer, buf_size);
    if (!img) 
    {
        std::cerr << "[ERROR] Could not load image into FBG" << std::endl;
        return;
    }
        
    fbg_imageClip(fbg_, img, 0, 0, 0, 0, fbg_->width, fbg_->height);

    fbg_flip(fbg_); // I don't know if this is necessary here

    fbg_draw(fbg_);

    // Free resources
    fbg_freeImage(img);
    free(buffer);
}

void FbDisplay::clear()
{
   fbg_clear(fbg_);
   fbg_draw(fbg_); 
}