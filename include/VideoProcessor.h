#pragma once

#include "VideoProvider.h"

class VideoProcessor : public VideoProvider
{
public:
    VideoProcessor();
    ~VideoProcessor();

    bool init();

    void setImageCallback(std::function<void(const cv::Mat&)> callback);

    // Methods to adjust video from HUD class
    void setZoom(uint8_t zoom);
    void setBrightness(uint8_t brightness_percent);
    void setZoomOffset(const int& x_offset, const int& y_offset);

private:
    void onImage(const cv::Mat& frame) override;
    
    void resizeFrame(cv::Mat& frame, int size_x, int size_y);

    void applyCrosshair(cv::Mat& frame);

    void applyZoom(cv::Mat& frame);

    void applyBrightness(cv::Mat& frame);

    //void applyEdgeDetection(cv::Mat& frame, double threshold1 = 100, double threshold2 = 200);

    //void applyContourFilter(cv::Mat& image);
    /*
    // Basic image processing operations
    void applyGaussianBlur(cv::Mat& frame, int kernelSize = 15);
    void applyGrayscale(cv::Mat& frame);
    void applyEdgeDetection(cv::Mat& frame, double threshold1 = 100, double threshold2 = 200);
    void applyColorFilter(cv::Mat& frame, const cv::Scalar& lowerBound, const cv::Scalar& upperBound);
    
    // Brightness and contrast adjustment
    void adjustBrightnessContrast(cv::Mat& frame, double alpha = 1.0, int beta = 0);
    
    // Histogram equalization
    void equalizeHistogram(cv::Mat& frame);
    
    // Noise reduction
    void reduceNoise(cv::Mat& frame);
    
    // Apply custom filter
    void applyCustomKernel(cv::Mat& frame, const cv::Mat& kernel);
    */
private:
    std::function<void(const cv::Mat&)> imageCallback_;

    uint8_t zoom_;
    int zoom_x_offset_;
    int zoom_y_offset_;


    uint8_t brightness_percent_;
};
