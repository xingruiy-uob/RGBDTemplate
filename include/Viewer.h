#pragma once
#include <memory>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <pangolin/pangolin.h>

#include "System.h"

namespace SLAM
{

class System;

class Viewer
{
public:
    Viewer(const std::string &strSettingsPath, System *pSystem);

    void Run();
    void Reset();

    void SetImageRGB(const cv::Mat &ImgRGB);
    void SetImageDepth(const cv::Mat &ImgDepth);
    void SetCameraPose(const Eigen::Matrix4d &Tcw);

private:
    void DrawCameraFrustum();

    System *mpSystem;

    int mWidth, mHeight;
    Eigen::Matrix4d mTcw;
    Eigen::Matrix3d mCameraMatrix;

    pangolin::GlTexture mTexRGB;
    pangolin::GlTexture mTexDepth;

    pangolin::View *mpRightBar;
    pangolin::View *mpRGBView;
    pangolin::View *mpMapView;
    pangolin::View *mpDepthView;

    cv::Mat mImgRGB;
    cv::Mat mImgDepth;
};

} // namespace SLAM
