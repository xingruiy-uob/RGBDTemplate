#include "System.h"

namespace SLAM
{

System::System(const std::string &strSettingsPath) : mbIsAlive(true), mbReverseRGB(false), mDepthScale(0.001)
{
    cv::FileStorage settingsFile(strSettingsPath, cv::FileStorage::READ);

    if (settingsFile.isOpened())
    {
        mbReverseRGB = (int)settingsFile["System.ReverseRGB"] == 0;
        mDepthScale = 1.0 / (float)settingsFile["System.DepthScale"];

        if ((int)settingsFile["System.EnableViewer"] == 1)
        {
            mpViewer = new Viewer(strSettingsPath, this);
            mViewerThread = new std::thread(&Viewer::Run, mpViewer);
        }

        mpTracker = new Tracking(strSettingsPath, mpViewer);
    }
    else
    {
        printf("Reading settings failed at line %d in file %s\n", __LINE__, __FILE__);
        exit(-1);
    }
}

System::~System()
{
    mViewerThread->join();
    delete mpViewer;
    delete mpTracker;
}

void System::TrackImage(const cv::Mat &ImgRGB, const cv::Mat &ImgDepth, const double TimeStamp)
{
    cv::Mat ImgGray;
    cv::Mat ImgDepthFloat;

    if (mbReverseRGB)
        cv::cvtColor(ImgRGB, ImgGray, cv::COLOR_BGR2GRAY);
    else
        cv::cvtColor(ImgRGB, ImgGray, cv::COLOR_RGB2GRAY);

    ImgDepth.convertTo(ImgDepthFloat, CV_32FC1, mDepthScale);

    mpTracker->TrackImage(ImgDepthFloat, ImgGray, TimeStamp);
}

bool System::IsAlive() const
{
    return mbIsAlive;
}

void System::Kill()
{
    mbIsAlive = false;
    printf("System Killed.\n");
}

void System::Reset()
{
    printf("Reset Complete.\n");
}

} // namespace SLAM