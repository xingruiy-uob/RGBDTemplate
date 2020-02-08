#pragma once
#include <iostream>
#include <Eigen/Core>
#include <opencv2/opencv.hpp>

#include "Map.h"
#include "Frame.h"
#include "Viewer.h"
#include "KeyFrame.h"

namespace SLAM
{

class Viewer;

class Tracking
{
public:
    Tracking(const std::string &strSettingsPath, Map *pMap, Viewer *pViewer);
    void TrackImage(const cv::Mat &ImDepth, const cv::Mat &ImGray, const double TimeStamp);

    void Reset();

private:
    enum class TrackingState
    {
        NotInitiated,
        OK,
        Lost
    };

    void Initialization();
    bool Relocalization();
    bool TrackWithLastFrame();
    bool TrackWithReferenceKF();

    Map *mpMap;
    Viewer *mpViewer;

    int mWidth, mHeight;
    Eigen::Matrix3d mCameraMatrix;
    TrackingState mCurrentState;

    Frame mLastFrame;
    Frame mCurrentFrame;
    KeyFrame mCurrentKeyFrame;
};

} // namespace SLAM