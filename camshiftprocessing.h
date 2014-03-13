#ifndef CAMSHIFTPROCESSING_H
#define CAMSHIFTPROCESSING_H


#include "videoprocessorclass.h"
#include "maskforcamshift.h"
#include "lkpyramid.h"

class CamshiftProcessing
{
public:
    CamshiftProcessing();
    void GetColorProbabilityMask();
    void GetOpticalFlow();
    //FaceDetectionClass* faceDetection;

private:
     VideoProcessorClass* videoProcessorClass;
     LKPyramid*             lkPyramidClass;
     MinMaxHSVValue maskValue;
     cv::Mat CurrentFrame;


};

#endif // CAMSHIFTPROCESSING_H
