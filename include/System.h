#pragma once
#include <thread>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "Viewer.h"
#include "Tracking.h"

namespace SLAM
{

class Viewer;
class Tracking;

class System
{
public:
    ~System();
    System(const std::string &strSettingsPath);
    void TrackImage(const cv::Mat &ImgRGB, const cv::Mat &ImgDepth, const double TimeStamp);

    bool IsAlive() const;
    void Kill();
    void Reset();

private:
    Viewer *mpViewer;
    Tracking *mpTracker;

    std::thread *mViewerThread;

    bool mbIsAlive;
    bool mbReverseRGB;
    float mDepthScale;
};

} // namespace SLAM