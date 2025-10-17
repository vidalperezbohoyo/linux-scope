#include "VideoProcessor.h"

VideoProcessor::VideoProcessor()
{
    zoom_ = 2; // No zoom
    brightness_percent_ = 20; // No brightness adjustment
}

VideoProcessor::~VideoProcessor()
{
}

bool VideoProcessor::init()
{
    return VideoProvider::init();
}

void VideoProcessor::setImageCallback(std::function<void(const cv::Mat&)> callback)
{
    imageCallback_ = callback;
}

void VideoProcessor::onImage(const cv::Mat& frame)
{
    // Copy image
    cv::Mat processed = frame.clone();

    // Apply zoom
    applyZoom(processed);

    // Apply brightness adjustment
    applyBrightness(processed);

    // Call the image callback if set
    if (imageCallback_)
    {
        imageCallback_(processed);
    }
}

void VideoProcessor::applyZoom(cv::Mat& frame)
{
    if (frame.empty() || zoom_ <= 1.0) return;

    int width = frame.cols;
    int height = frame.rows;

    int new_width = static_cast<int>(width / zoom_);
    int new_height = static_cast<int>(height / zoom_);

    int center_x = width / 2;
    int center_y = height / 2;

    int x = std::max(0, center_x - new_width / 2);
    int y = std::max(0, center_y - new_height / 2);

    // Ensure ROI stays within bounds
    new_width = std::min(new_width, width - x);
    new_height = std::min(new_height, height - y);

    cv::Rect roi(x, y, new_width, new_height);
    cv::Mat cropped = frame(roi);

    // Resize with interpolation to avoid visual artifacts
    cv::resize(cropped, frame, frame.size(), 0, 0, cv::INTER_LINEAR);
}

void VideoProcessor::applyBrightness(cv::Mat& frame)
{
    if (frame.empty()) return;

    brightness_percent_ = std::clamp(brightness_percent_, (uint8_t)0, (uint8_t)100);
    double factor = brightness_percent_ / 100.0;

    frame.convertTo(frame, -1, factor, 0); // alpha = factor, beta = 0
}

/*
void VideoProcessor::applyEdgeDetection(cv::Mat& frame, double threshold1, double threshold2)
{
    if (frame.empty()) return;
    
    cv::Mat gray, edges;
    
    // Convert to grayscale
    if (frame.channels() == 3)
    {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray = frame.clone();
    }
    
    // Apply Canny edge detection
    cv::Canny(gray, edges, threshold1, threshold2);
    
    // Convert back to 3 channels
    cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
}

void VideoProcessor::applyContourFilter(cv::Mat& image)
{
    if (image.empty()) return;

    cv::Mat gray, binary;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::threshold(gray, binary, 20, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Point center(binary.cols / 2, binary.rows / 2);
    double image_area = static_cast<double>(binary.cols * binary.rows);
    double area_limit = 0.3 * image_area;

    double max_area = 0.0;
    int best_index = -1;

    for (size_t i = 0; i < contours.size(); ++i) {
        if (cv::pointPolygonTest(contours[i], center, false) >= 0) {
            double area = cv::contourArea(contours[i]);
            if (area > max_area && area <= area_limit) {
                max_area = area;
                best_index = static_cast<int>(i);
            }
        }
    }

    if (best_index >= 0) {
        cv::drawContours(image, contours, best_index, cv::Scalar(0, 255, 0), 2);
    }
}

void VideoProcessor::applyGaussianBlur(cv::Mat& frame, int kernelSize)
{
    if (frame.empty()) return;
    
    // Ensure kernel size is odd
    if (kernelSize % 2 == 0) kernelSize++;
    
    cv::GaussianBlur(frame, frame, cv::Size(kernelSize, kernelSize), 0);
}

void VideoProcessor::applyGrayscale(cv::Mat& frame)
{
    if (frame.empty()) return;
    
    if (frame.channels() == 3)
    {
        cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
        cv::cvtColor(frame, frame, cv::COLOR_GRAY2BGR); // Convert back to 3 channels for consistency
    }
}

void VideoProcessor::applyEdgeDetection(cv::Mat& frame, double threshold1, double threshold2)
{
    if (frame.empty()) return;
    
    cv::Mat gray, edges;
    
    // Convert to grayscale
    if (frame.channels() == 3)
    {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray = frame.clone();
    }
    
    // Apply Canny edge detection
    cv::Canny(gray, edges, threshold1, threshold2);
    
    // Convert back to 3 channels
    cv::cvtColor(edges, frame, cv::COLOR_GRAY2BGR);
}

void VideoProcessor::applyColorFilter(cv::Mat& frame, const cv::Scalar& lowerBound, const cv::Scalar& upperBound)
{
    if (frame.empty()) return;
    
    cv::Mat hsv, mask;
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);
    
    cv::inRange(hsv, lowerBound, upperBound, mask);
    cv::bitwise_and(frame, frame, frame, mask);
}

void VideoProcessor::adjustBrightnessContrast(cv::Mat& frame, double alpha, int beta)
{
    if (frame.empty()) return;
    
    frame.convertTo(frame, -1, alpha, beta);
}

void VideoProcessor::equalizeHistogram(cv::Mat& frame)
{
    if (frame.empty()) return;
    
    if (frame.channels() == 1)
    {
        cv::equalizeHist(frame, frame);
    }
    else if (frame.channels() == 3)
    {
        cv::Mat yuv;
        cv::cvtColor(frame, yuv, cv::COLOR_BGR2YUV);
        
        std::vector<cv::Mat> channels;
        cv::split(yuv, channels);
        
        cv::equalizeHist(channels[0], channels[0]);
        
        cv::merge(channels, yuv);
        cv::cvtColor(yuv, frame, cv::COLOR_YUV2BGR);
    }
}

std::vector<cv::Rect> VideoProcessor::detectObjects(const cv::Mat& frame)
{
    std::vector<cv::Rect> objects;
    
    if (frame.empty() || !m_detectorLoaded) return objects;
    
    cv::Mat gray;
    if (frame.channels() == 3)
    {
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    }
    else
    {
        gray = frame.clone();
    }
    
    m_objectDetector.detectMultiScale(gray, objects, 1.1, 3, 0, cv::Size(30, 30));
    
    return objects;
}

void VideoProcessor::trackObjects(cv::Mat& frame, const std::vector<cv::Rect>& objects)
{
    if (frame.empty()) return;
    
    // Simple tracking visualization - draw rectangles around detected objects
    for (const auto& obj : objects)
    {
        cv::rectangle(frame, obj, cv::Scalar(0, 255, 0), 2);
    }
}

void VideoProcessor::reduceNoise(cv::Mat& frame)
{
    if (frame.empty()) return;
    
    cv::Mat denoised;
    cv::fastNlMeansDenoisingColored(frame, denoised, 10, 10, 7, 21);
    frame = denoised;
}

void VideoProcessor::applyCustomKernel(cv::Mat& frame, const cv::Mat& kernel)
{
    if (frame.empty() || kernel.empty()) return;
    
    cv::filter2D(frame, frame, -1, kernel);
}
*/