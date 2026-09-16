#include "Display/CvDisplay.h"

CvDisplay::CvDisplay()
{
}

CvDisplay::~CvDisplay()
{
}

bool CvDisplay::init()
{
    cv::namedWindow("Video", cv::WINDOW_AUTOSIZE);
    cv::resizeWindow("Video", 240, 240);

    return true;
}

void CvDisplay::draw(const cv::Mat& frame)
{
    if (frame.empty()) return;
    
    cv::Mat result = cv::Mat::zeros(frame.size(), frame.type());
    cv::Mat mask = cv::Mat::zeros(frame.size(), CV_8UC1);

    cv::Point center(frame.cols / 2, frame.rows / 2);
    int radius = std::min(frame.cols, frame.rows) / 2;

    cv::circle(mask, center, radius, cv::Scalar(255), cv::FILLED);

    // Copia únicamente la zona del círculo
    frame.copyTo(result, mask);

    cv::imshow("Video", result);
    cv::waitKey(1);
}

void CvDisplay::clear()
{
    cv::Mat blackFrame = cv::Mat::zeros(240, 240, CV_8UC3);
    cv::imshow("Video", blackFrame);
    cv::waitKey(1); // Needed to update the window
}