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
    if (frame.empty()) return;

    cv::imshow("Video", frame);
    cv::waitKey(1);
}

void CvDisplay::clear()
{
    cv::Mat blackFrame = cv::Mat::zeros(240, 240, CV_8UC3);
    cv::imshow("Video", blackFrame);
    cv::waitKey(1); // Needed to update the window
}