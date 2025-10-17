#include "FbDisplay.h"

bool FbDisplay::init()
{
   fbg_ = fbg_fbdevSetup(DEVICE_PATH.c_str(), 0); 
   return fbg_ != nullptr; 
}

void FbDisplay::draw(const cv::Mat& frame)
{
    uint8_t* buffer = nullptr;

    cv::Mat resized;
    resize(frame, resized, Size(fbg_->width, fbg_->height));
    cvtColor(resized, resized, COLOR_BGR2RGB);

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
    struct _fbg_img* img = fbg_loadImageFromMemory(fbg, buffer, buf_size);
    if (!img) 
    {
        std::cerr << "[ERROR] Could not load image into FBG" << std::endl;
        continue;
    }
        
    fbg_imageClip(fbg, img, 0, 0, 0, 0, fbg->width, fbg->height);

    fbg_flip(fbg); // I don't know if this is necessary here

    fbg_draw(fbg);

    // Free resources
    fbg_freeImage(img);
    free(buffer);
}

void FbDisplay::clear()
{
   fbg_clear(fbg_);
   fbg_draw(fbg); 
}