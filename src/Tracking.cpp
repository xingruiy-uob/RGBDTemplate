#include "Tracking.h"

namespace SLAM
{

Tracking::Tracking(const std::string &strSettingsPath, Viewer *pViewer) : mpViewer(pViewer)
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

void Tracking::TrackImage(const cv::Mat &ImgDepth, const cv::Mat &ImgGray, const double TimeStamp)
{
    cv::imshow("Gray", ImgGray);
    cv::imshow("Depth", ImgDepth);
    cv::waitKey(1);

    if (mpViewer)
    {
        auto Tcw = Eigen::Matrix4d::Identity();
        mpViewer->SetCameraPose(Tcw);
    }
}

} // namespace SLAM
