#include "VideoProcessor.h"

VideoProcessor::VideoProcessor()
{
    // Try to load a default object detector (face detector)
    // Note: You'll need to provide the path to the cascade file
    // m_detectorLoaded = m_objectDetector.load("haarcascade_frontalface_alt.xml");
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

    // Apply processing steps
    //applyEdgeDetection(processed);
    applyContourFilter(processed);

    // Call the image callback if set
    if (imageCallback_)
    {
        imageCallback_(processed);
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

void VideoProcessor::applyContourFilter(cv::Mat& image)
{
    if (image.empty()) return;

    cv::Mat gray, binary;
    if (image.channels() == 3) {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    } else {
        gray = image.clone();
    }

    cv::threshold(gray, binary, 127, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(binary, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Point center(binary.cols / 2, binary.rows / 2);

    double max_area = 0.0;
    int best_index = -1;

    for (size_t i = 0; i < contours.size(); ++i) {
        if (cv::pointPolygonTest(contours[i], center, false) >= 0) {
            double area = cv::contourArea(contours[i]);
            if (area > max_area) {
                max_area = area;
                best_index = static_cast<int>(i);
            }
        }
    }

    image = cv::Mat::zeros(image.size(), CV_8UC3);

    if (best_index >= 0) {
        cv::drawContours(image, contours, best_index, cv::Scalar(0, 255, 0), 2);
    }
}

/*
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