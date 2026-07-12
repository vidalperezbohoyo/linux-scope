#include "Display/HUD.h"

HUD::HUD()
{

}

HUD::~HUD()
{

}

void HUD::draw(cv::Mat& in_240p, cv::Mat& out_240p)
{
    // Copy input to output
    in_240p.copyTo(out_240p);

    // Draw crosshair
    int cx = out_240p.cols / 2;
    int cy = out_240p.rows / 2;

    cv::line(out_240p, cv::Point(0, cy), cv::Point(out_240p.cols, cy), crosshair_color, 1);
    cv::line(out_240p, cv::Point(cx, 0), cv::Point(cx, out_240p.rows), crosshair_color, 1);

    // Draw magnification and distance text
    std::string magnification_text = std::to_string(magnification) + "x";
    std::string distance_text = std::to_string(distance) + "m";

    int font_face = cv::FONT_HERSHEY_SIMPLEX;
    double font_scale = 0.5;
    int thickness = 1;

    int baseline = 0;
    cv::Size magnification_size = cv::getTextSize(magnification_text, font_face, font_scale, thickness, &baseline);
    cv::Size distance_size = cv::getTextSize(distance_text, font_face, font_scale, thickness, &baseline);

    cv::putText(out_240p, magnification_text, cv::Point(5, 20), font_face, font_scale, crosshair_color, thickness);
    cv::putText(out_240p, distance_text, cv::Point(5, 40), font_face, font_scale, crosshair_color, thickness);
}