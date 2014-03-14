#include "camshiftprocessing.h"

CamshiftProcessing::CamshiftProcessing()
{
    videoProcessorClass = VideoProcessorClass::getInstance();
    lkPyramidClass      = new LKPyramid();
    motionMask = cv::Mat();
}


void CamshiftProcessing::GetColorProbabilityMask() {
    if(videoProcessorClass->facedetectionClass != NULL) {
        HSVmaskValue = videoProcessorClass->facedetectionClass->GetHSVmaskForFace();
    }
    else {
        std::cout << "No processing has been done to get the HSV value"  << std::endl;
        return;
    }
}


cv::Mat CamshiftProcessing::GetOpticalFlow() {
    lkPyramidClass->CalculateOpticalFlow(CurrentFrame.clone());
    motionMask = lkPyramidClass->GetTheMotionMask();
    return motionMask;
}

void CamshiftProcessing::TrackRegionOfInterest() {
    lkPyramidClass->SetCurrentFrameWindow("current frame optical window");
    lkPyramidClass->SetPrevFrameWindow("previous frame optical flow window");
    lkPyramidClass->SetOpticalFlowWindow("optical flow window");
    GetColorProbabilityMask();
    while(true) {
        videoProcessorClass->capture.read(CurrentFrame);
        GetOpticalFlow();


        cv::minMaxLoc(motionMask, minDisplacement, maxDisplacement, minDisplacementLocation, maxDisplacementLocation);

        cv::cvtColor(CurrentFrame, HSVFrame, CV_BGR2HSV);
        cv::inRange(HSVFrame, cv::Scalar(HSVmaskValue.minhue, HSVmaskValue.minSat, HSVmaskValue.minValue),
                    cv::Scalar(HSVmaskValue.maxhue, HSVmaskValue.maxSat, HSVmaskValue.maxvalue),maskToCurrentFrame);
        int ch = {0, 0};
        HueFrame.create(HSVFrame.size(), HSVFrame.depth());
        cv::mixChannels(&HSVFrame, 1, &HueFrame, 1, ch, 1);

        //cv::calcHist(HueFrame, 1, 0, maskToCurrentFrame, )

        CurrentFrame.release();

        int c = cv::waitKey(10);
        if(c==27)  //ESC key
            break;
    }

}

