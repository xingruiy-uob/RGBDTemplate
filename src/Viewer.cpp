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

        pangolin::CreateWindowAndBind("SLAM", mWidth, mHeight);
        pangolin::GetBoundWindow()->RemoveCurrent();
    }
    else
    {
        printf("Reading settings failed at line %d in file %s\n", __LINE__, __FILE__);
        exit(-1);
    }
}

void Viewer::Reset()
{
    mTcw = Eigen::Matrix4d::Identity();
}

void Viewer::Run()
{
    pangolin::BindToContext("SLAM");

    mTexRGB.Reinitialise(
        640, 480,
        GL_RGB,
        true,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        NULL);

    mTexDepth.Reinitialise(
        640, 480,
        GL_LUMINANCE,
        true,
        0,
        GL_LUMINANCE,
        GL_FLOAT,
        NULL);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto RenderState = pangolin::OpenGlRenderState(
        pangolin::ProjectionMatrix(1024, 768, 500, 500, 512, 389, 0.1, 1000),
        pangolin::ModelViewLookAtRDF(0, 0, 0, 0, 0, -1, 0, 1, 0));

    auto MenuDividerLeft = pangolin::Attach::Pix(200);
    float RightSideBarDividerLeft = 0.75f;

    mpMapView = &pangolin::Display("Map");
    mpMapView->SetBounds(0, 1, MenuDividerLeft, RightSideBarDividerLeft)
        .SetHandler(new pangolin::Handler3D(RenderState));
    mpRightBar = &pangolin::Display("RightBar");
    mpRightBar->SetBounds(0, 1, RightSideBarDividerLeft, 1);

    mpRGBView = &pangolin::Display("RGB");
    mpRGBView->SetBounds(0, 0.33, 0, 1);
    mpDepthView = &pangolin::Display("Depth");
    mpDepthView->SetBounds(0.33, 0.66, 0, 1);

    mpRightBar->AddDisplay(*mpRGBView);
    mpRightBar->AddDisplay(*mpDepthView);

    pangolin::CreatePanel("menu").SetBounds(0, 1, 0, MenuDividerLeft);
    pangolin::Var<bool> varReset = pangolin::Var<bool>("menu.Reset", false, false);

    while (!pangolin::ShouldQuit())
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (pangolin::Pushed(varReset))
            mpSystem->Reset();

        DrawTextures();

        mpMapView->Activate(RenderState);
        DrawCameraFrustum();

        pangolin::FinishFrame();
    }

    mpSystem->Kill();
}

void Viewer::SetCameraPose(const Eigen::Matrix4d &Tcw)
{
    mTcw = Tcw;
}

void Viewer::DrawTextures()
{
    mpRGBView->Activate();
    if (!mImgRGB.empty())
        mTexRGB.Upload(mImgRGB.data, GL_RGB, GL_UNSIGNED_BYTE);
    mTexRGB.RenderToViewportFlipY();

    mpDepthView->Activate();
    if (!mImgDepth.empty())
        mTexDepth.Upload(mImgDepth.data, GL_LUMINANCE, GL_FLOAT);
    mTexDepth.RenderToViewportFlipY();
}

void Viewer::DrawCameraFrustum()
{
    glColor3f(1.0, 0.0, 0.0);
    pangolin::glDrawFrustum<double>(mCameraMatrix.inverse(), 640, 480, mTcw, 0.1);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void Viewer::SetImageRGB(const cv::Mat &ImgRGB)
{
    mImgRGB = ImgRGB;
}

void Viewer::SetImageDepth(const cv::Mat &ImgDepth)
{
    mImgDepth = ImgDepth;
}

} // namespace SLAM