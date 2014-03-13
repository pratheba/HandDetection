#include "camshiftprocessing.h"

CamshiftProcessing::CamshiftProcessing()
{
    videoProcessorClass = VideoProcessorClass.getInstance();
    lkPyramidClass      = new LKPyramid();
}


void CamshiftProcessing::GetColorProbabilityMask() {
    if(videoProcessor->facedetectionClass != NULL) {
        //std::cout << videoProcessor->facedetectionClass->regionOfInterestinHSV.size() << std::endl;
        maskValue = videoProcessor->facedetectionClass->GetHSVmaskForFace();
    }
    else {
        std::cout << "No processing has been done to get the HSV value"  << std::endl;
        return;
        //videoProcessor->facedetectionClass = new FaceDetectionClass();
        //maskValue = videoProcessor->facedetectionClass->GetHSVmaskForFace();
    }
}


void CamshiftProcessing::GetOpticalFlow() {
    while(true) {
        videoProcessorClass->capture.read(CurrentFrame);
        lkPyramidClass->CalculateOpticalFlow(CurrentFrame);

        CurrentFrame.release();

        int c = cv::waitKey(10);
        if(c==27)  //ESC key
            break;
    }
}

void CamshiftProcessing::TrackRegionOfInterest() {

}
