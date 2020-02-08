#include "Tracking.h"

namespace SLAM
{

Tracking::Tracking(const std::string &strSettingsPath, Map *pMap, Viewer *pViewer)
    : mpMap(pMap), mpViewer(pViewer)
{
    cv::FileStorage settingsFile(strSettingsPath, cv::FileStorage::READ);

    if (settingsFile.isOpened())
    {
        mWidth = settingsFile["Tracking.Width"];
        mHeight = settingsFile["Tracking.Height"];

        mCameraMatrix = Eigen::Matrix3d::Identity();
        mCameraMatrix(0, 0) = (double)settingsFile["Tracking.Fx"];
        mCameraMatrix(1, 1) = (double)settingsFile["Tracking.Fy"];
        mCameraMatrix(0, 2) = (double)settingsFile["Tracking.Cx"];
        mCameraMatrix(1, 2) = (double)settingsFile["Tracking.Cy"];
    }
    else
    {
        printf("Reading settings failed at line %d in file %s\n", __LINE__, __FILE__);
        exit(-1);
    }
}

void Tracking::Reset()
{
    mCurrentState = TrackingState::NotInitiated;
}

void Tracking::TrackImage(const cv::Mat &ImgDepth, const cv::Mat &ImgGray, const double TimeStamp)
{
    switch (mCurrentState)
    {
    case TrackingState::NotInitiated:
    {
        Initialization();
        break;
    }
    case TrackingState::OK:
    {
        bool bOK = TrackWithReferenceKF();

        if (!bOK)
            mCurrentState = TrackingState::Lost;
        break;
    }

    case TrackingState::Lost:
    {
        bool bOK = Relocalization();
        break;
    }
    }

    if (mpViewer)
    {
        auto Tcw = Eigen::Matrix4d::Identity();
        mpViewer->SetCameraPose(Tcw);
    }
}

void Tracking::Initialization()
{
    mCurrentState = TrackingState::OK;
}

bool Tracking::Relocalization()
{
}

bool Tracking::TrackWithLastFrame()
{
}

bool Tracking::TrackWithReferenceKF()
{
}

} // namespace SLAM
