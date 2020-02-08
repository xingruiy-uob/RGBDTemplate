#pragma once
#include <iostream>
#include <Eigen/Core>
#include <opencv2/opencv.hpp>

#include "Viewer.h"

namespace SLAM
{

class Viewer;

class Tracking
{
public:
    Tracking(const std::string &strSettingsPath, Viewer *pViewer);
    void TrackImage(const cv::Mat &ImDepth, const cv::Mat &ImGray, const double TimeStamp);

private:
    Viewer *mpViewer;

    int mWidth, mHeight;
    Eigen::Matrix3d mCameraMatrix;
};

} // namespace SLAM