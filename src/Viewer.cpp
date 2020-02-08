#include "Viewer.h"

namespace SLAM
{

Viewer::Viewer(const std::string &strSettingsPath, System *pSystem) : mpSystem(pSystem), mTcw(Eigen::Matrix4d::Identity())
{
    cv::FileStorage settingsFile(strSettingsPath, cv::FileStorage::READ);

    if (settingsFile.isOpened())
    {
        mWidth = settingsFile["Viewer.Width"];
        mHeight = settingsFile["Viewer.Height"];

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

void Viewer::Run()
{
    pangolin::CreateWindowAndBind("SLAM", mWidth, mHeight);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    pangolin::CreatePanel("menu").SetBounds(0.0, 1.0, 0.0, pangolin::Attach::Pix(175));
    pangolin::Var<bool> menuResetSystem("menu.Reset", true, false);

    // Define Camera Render Object (for view / scene browsing)
    pangolin::OpenGlRenderState s_cam(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
        pangolin::ModelViewLookAt(0, 0, -1, 0, 0, 0, 0.0, -1.0, 0.0));

    // Add named OpenGL viewport to window and provide 3D Handler
    pangolin::View &d_cam = pangolin::CreateDisplay()
                                .SetBounds(0.0, 1.0, pangolin::Attach::Pix(175), 1.0, -1024.0f / 768.0f)
                                .SetHandler(new pangolin::Handler3D(s_cam));

    while (!pangolin::ShouldQuit())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (pangolin::Pushed(menuResetSystem))
            mpSystem->Reset();

        d_cam.Activate(s_cam);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        DrawCameraFrustum();

        pangolin::FinishFrame();
    }

    mpSystem->Kill();
}

void Viewer::SetCameraPose(const Eigen::Matrix4d &Tcw)
{
    mTcw = Tcw;
}

void Viewer::DrawCameraFrustum()
{
    glColor3f(1.0, 0.0, 0.0);
    pangolin::glDrawFrustum<double>(mCameraMatrix.inverse(), 640, 480, mTcw, 0.1);
}

} // namespace SLAM
