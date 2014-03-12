#ifndef CAMSHIFTPROCESSING_H
#define CAMSHIFTPROCESSING_H

#include "facedetectionclass.h"
#include "videoprocessorclass.h"

class CamshiftProcessing
{
public:
    CamshiftProcessing();
    void GetColorProbabilityMask(VideoProcessorClass* videoProcessor);
    //FaceDetectionClass* faceDetection;

private:
     //VideoProcessorClass *videoProcessor;


};

#endif // CAMSHIFTPROCESSING_H
