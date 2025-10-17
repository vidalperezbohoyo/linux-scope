#include "CvDisplay.h"

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
    cv::Mat resized;
    cv::resize(frame, resized, cv::Size(240, 240));
    cv::imshow("Video", resized);
    cv::waitKey(1); // Needed to update the window
}

void CvDisplay::clear()
{
    cv::Mat blackFrame = cv::Mat::zeros(240, 240, CV_8UC3);
    cv::imshow("Video", blackFrame);
    cv::waitKey(1); // Needed to update the window
}