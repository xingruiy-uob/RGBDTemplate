#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

namespace SLAM
{

class System
{
public:
    System(const std::string &ConfigFilePath);
    void TrackImage(const cv::Mat &ImgDepth, const cv::Mat &ImgRGB, const double TimeStamp);

private:
};

} // namespace SLAM