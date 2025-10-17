#pragma once

#include <opencv2/opencv.hpp>
#include <vector>

class VideoProcessor : public VideoProvider
{
public:
    VideoProcessor();
    ~VideoProcessor();

private:
    void onFrame(cv::Mat& frame); 

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
};
