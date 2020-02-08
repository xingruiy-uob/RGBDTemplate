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

    while (slam.IsAlive())
    {
        if (cam.TryFetchingImages(ImDepth, ImRGB))
        {
            slam.TrackImage(ImRGB, ImDepth, 0);
        }
    }
}
