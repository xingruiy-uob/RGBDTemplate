#pragma once
#include <iostream>
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

    void SetCameraPose(const Eigen::Matrix4d &Tcw);

private:
    void DrawCameraFrustum();

    System *mpSystem;

    int mWidth, mHeight;
    Eigen::Matrix4d mTcw;
    Eigen::Matrix3d mCameraMatrix;
};

} // namespace SLAM
