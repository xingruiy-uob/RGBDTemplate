#include "System.h"
#include "CameraOpenNI.h"

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        exit(-1);
    }

    ONI::Camera cam;
    SLAM::System slam(argv[1]);

    cv::Mat ImDepth, ImRGB;

    while (true)
    {
        if (cam.TryFetchingImages(ImDepth, ImRGB))
        {
            cv::imshow("RGB", ImRGB);
            cv::imshow("Depth", ImDepth);
            cv::waitKey(1);

            slam.TrackImage(ImRGB, ImDepth, 0);
        }
    }
}
