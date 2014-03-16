#include "camshiftprocessing.h"



CamshiftProcessing::CamshiftProcessing():histogramranges({0,180}), backprojMode(false), flowMode(false)
{
    videoProcessorClass = VideoProcessorClass::getInstance();
    motionMaskClass = new MotionDetectionClass();
    ROIClass = new RegionOfInterest();
    motionMask = cv::Mat();
    cv::namedWindow("ROI",0);
    IsFirstSelection = true;
    histogramsize = 180;
}

void CamshiftProcessing::GetColorProbabilityMask() {
    if(videoProcessorClass->facedetectionClass != NULL) {
        HSVmaskValue = videoProcessorClass->facedetectionClass->GetHSVmaskForFace();
    }
    else {float* phranges;

        std::cout << "No processing has been done to get the HSV value"  << std::endl;
        return;
    }
}

void CamshiftProcessing::SetOpticalFlow() {
    if(motionMask.empty())
        motionMask = cv::Mat(CurrentFrame.rows, CurrentFrame.cols, CV_8U, cv::Scalar(1.0));
    motionMask = cv::Scalar(1.0);
    motionMask = motionMaskClass->GetTheMotionMask(CurrentFrame.clone());
}

void CamshiftProcessing::CalculateBackProjection() {
    const float* phranges = histogramranges;
    cv::calcBackProject(&HueFrame, 1, 0, histogramImage, backProjectImage, &phranges);
    backProjectImage &= HSVMask;
}

void CamshiftProcessing::CalculateHistogramForRegionOfInterest() {
    if(ROIFortracking.area() == 0)
        return;

    ROIFortracking &= cv::Rect(0,0, CurrentFrame.cols, CurrentFrame.rows);
    ROI = cv::Mat(HueFrame, ROIFortracking);
    ROIMask = cv::Mat(HSVMask, ROIFortracking);

    const float* phranges = histogramranges;
    cv::calcHist(&ROI, 1, 0, ROIMask, histogramImage, 1, &histogramsize, &phranges);
    cv::normalize(histogramImage, histogramImage, 0, 255, cv::NORM_MINMAX);

}

void CamshiftProcessing::GetHueFrameFromHSVFrame() {
    int channel[] = {0, 0};
    HueFrame.create(HSVFrame.size(), HSVFrame.depth());
    mixChannels(&HSVFrame, 1, &HueFrame, 1, channel, 1);
}

void CamshiftProcessing::SelectRegionOfInterest() {
    if(motionMask.empty())
        return;
     ROIClass->FindRegionOfInterestToTrack(motionMask);
     ROIClass->DrawBoundingBoxforRegionOfInterest(CurrentFrame);
     ROIFortracking = ROIClass->GetBoundingBoxOfRegionofInterest();
}

void CamshiftProcessing::SetHSVColorProbabilityMask() {
    cv::inRange(HSVFrame, cv::Scalar(HSVmaskValue.minhue, HSVmaskValue.minSat, HSVmaskValue.minValue),
            cv::Scalar(HSVmaskValue.maxhue, HSVmaskValue.maxSat, HSVmaskValue.maxvalue), HSVMask);

}

void CamshiftProcessing::ConvertBGRImageToHSV() {
    if(!CurrentFrame.empty())
        cv::cvtColor(CurrentFrame, HSVFrame, CV_BGR2HSV);
}

void CamshiftProcessing::DrawTrackingObject(cv::RotatedRect trackingBox) {

    if( trackingWindow.area() <= 1 )
    {
        int cols = finalProbabilityMask.cols, rows = finalProbabilityMask.rows, r = (MIN(cols, rows) + 5)/6;
        trackingWindow = cv::Rect(trackingWindow.x - r, trackingWindow.y - r,
                           trackingWindow.x + r, trackingWindow.y + r) &
                      cv::Rect(0, 0, cols, rows);
    }
     cv::ellipse( CurrentFrame, trackingBox, cv::Scalar(0,0,255), 3, CV_AA );

}

void CamshiftProcessing::InitializeCamshift() {
    ConvertBGRImageToHSV();
    SetHSVColorProbabilityMask();
    GetHueFrameFromHSVFrame();

    if(ROIFortracking.area()!=0 && IsFirstSelection) {
        CalculateHistogramForRegionOfInterest();
        trackingWindow = ROIFortracking;
        IsFirstSelection = false;
    }

}

void CamshiftProcessing::GetFinalProbabilityMask() {
    if(flowMode) {
        if(motionMask.rows != 0 && motionMask.cols !=0 )
            cv::addWeighted(backProjectImage, 0.4, motionMask, 0.9, 0.0, finalProbabilityMask);
    }
    else
        finalProbabilityMask = backProjectImage.clone();

        if( backprojMode )
            cv::cvtColor( finalProbabilityMask, CurrentFrame, CV_GRAY2BGR );
}

void CamshiftProcessing::ApplyFinalProbabilityMask() {
     cv::RotatedRect trackingBox = cv::CamShift(finalProbabilityMask, trackingWindow, cv::TermCriteria( cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 0.1));
     DrawTrackingObject(trackingBox);
}

void CamshiftProcessing::StartCamshift() {
    InitializeCamshift();
    CalculateBackProjection();
    GetFinalProbabilityMask();
    ApplyFinalProbabilityMask();
}

void CamshiftProcessing::ActionOnKeyPress(int key) {
    switch(key)
    {
    case 'b':
        backprojMode = !backprojMode;
        break;
    case 'f':
        flowMode = !flowMode;
        break;
    default:
        ;
    }
}

void CamshiftProcessing::TrackRegionOfInterest() {

    GetColorProbabilityMask();
    while(true) {
        videoProcessorClass->capture.read(CurrentFrame);
        if(CurrentFrame.empty())
            continue;
        SetOpticalFlow();
        if(motionMask.empty())
            continue;
        SelectRegionOfInterest();
        StartCamshift();

        char c = (char)cv::waitKey(10);
        if( c == 27 )
            break;
        ActionOnKeyPress(c);

    }
}



