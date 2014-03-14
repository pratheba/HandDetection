#ifndef CAMSHIFTPROCESSING_H
#define CAMSHIFTPROCESSING_H


#include "videoprocessorclass.h"
#include "maskforcamshift.h"
#include "lkpyramid.h"

class CamshiftProcessing
{
public:
    CamshiftProcessing();

    void TrackRegionOfInterest();
    //FaceDetectionClass* faceDetection;

private:
     VideoProcessorClass* videoProcessorClass;
     LKPyramid*             lkPyramidClass;
     MinMaxHSVValue HSVmaskValue;
     cv::Mat motionMask;
     cv::Mat CurrentFrame;
     cv::Mat HSVFrame;
     cv::Mat HueFrame;
     cv::Mat maskToCurrentFrame;

     double minDisplacement;
     double maxDisplacement;
     cv::Point minDisplacementLocation;
     cv::Point maxDisplacementLocation;

     void GetColorProbabilityMask();
     cv::Mat GetOpticalFlow();


};

#endif // CAMSHIFTPROCESSING_H
