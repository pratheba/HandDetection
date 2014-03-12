#include "camshiftprocessing.h"

CamshiftProcessing::CamshiftProcessing()//:faceDetection(NULL)
{
}


void CamshiftProcessing::GetColorProbabilityMask(VideoProcessorClass* videoProcessor) {
    if(videoProcessor->facedetectionClass != NULL) {
        //std::cout << videoProcessor->facedetectionClass->regionOfInterestinHSV.size() << std::endl;
        videoProcessor->facedetectionClass->GetHSVmaskForFace();
    }
    else {
        videoProcessor->facedetectionClass = new FaceDetectionClass();
        videoProcessor->facedetectionClass->GetHSVmaskForFace();
    }
}
