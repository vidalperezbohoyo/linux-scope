#pragma once

class FbDisplay : public BaseDisplay
{

private:
    const std::string DEVICE_PATH = "/dev/fb0";
public:
    FbDisplay();
    ~FbDisplay();
    
    bool init() override;
    
    void draw(const cv::Mat& frame) override;

    void clear();
};