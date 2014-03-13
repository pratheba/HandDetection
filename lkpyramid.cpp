#include "lkpyramid.h"

#define WINDOW_SIZE  21;
#define MAXFEATURECOUNT  500;

LKPyramid::LKPyramid()
{
    prevFrame   = cv::Mat();
    currentFrame = cv::Mat();
    CurrentFramefeatures.clear();
    PrevFramefeatures.clear();

    maxFeaturePoints = MAXFEATURECOUNT;
    featureQualityLevel = 0.01;
    minDistanceBetweenFeatures = 10;
    useHarrisDetector = false;
    valueForHarrisDetector = 0.04;
    windowSizeForFeatureSelection = 3;

    windowSizeForTracking = cv::Size(WINDOW_SIZE,WINDOW_SIZE);
    termcriteria = cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);

    videoProcessorClass = VideoProcessorClass.getInstance();

}

void SetCurrentFrameWindow(std::string windowNameCurr_) {
    windowNameCurr = windowNameCurr_;
    cv::namedWindow(windowNameCurr);
}

void SetPrevFrameWindow(std::string windowNamePrev_) {
    windowNamePrev = windownamePrev_;
    cv::namedWindow(windowNamePrev);
}

void SetOpticalFlowWindow(std::string windowNameOpticalFlow_) {
    windowNameOpticalFlow - windowNameOpticalFlow_;
    cv::namedWindow(windowNameOpticalFlow);
}

void LKPyramid::Initialize(cv::Mat& currentFrame_) {
    currentFrame = currentFrame_.clone();
    Initialize();
    GetFeaturesfortracking();
    GetCornerSubpixelfortracking();
}

void LKPyramid::Initialize() {
    if(prevFrame.empty()) {
        prevFrame = currentFrame.clone();
        GetFeaturesfortracking();
        GetCornerSubpixelfortracking();
        CurrentFramefeatures = PrevFramefeatures;
    }
}

void LKPyramid::GetFeaturesfortracking() {
    cv::goodFeaturesToTrack(prevFrame, PrevFramefeatures,
                            maxFeaturePoints, featureQualityLevel,
                            minDistanceBetweenFeatures, cv::Mat(), windowSizeForFeatureSelection,
                            useHarrisDetector, valueForHarrisDetector);

}


void LKPyramid::GetCornerSubpixelfortracking() {
    cv::cornerSubPix(prevFrame, PrevFramefeatures, windowSizeForTracking, cv::Size(-1, -1), termcriteria);
}

void LKPyramid::CalculateOpticalFlow(cv::Mat currentFrame_) {
    currentFrame = currentFrame_.clone();
    CurrentFramefeatures.clear();

    if(prevFrame.empty())
        Initialize();
    else {
        if(PrevFramefeatures.size() > 0) {
            GetFeaturesfortracking();
            GetCornerSubpixelfortracking();
            cv::calcOpticalFlowPyrLK(prevFrame, currentFrame,
                                     PrevFramefeatures, CurrentFramefeatures,
                                     statusOfFlowForFeature, error,
                                     windowSizeForTracking, 3, termcriteria);
        }
    }
    DrawTheFlow();
    DisplayFlow();

    cv::swap(prevFrame, currentFrame);
    std::swap(PrevFramefeatures, CurrentFramefeatures);
    currentFrame.release();

}

void LKPyramid::DrawTheFlow() {
    OpticalFlowImage.release();

    OpticalFlowImage = currentFrame.clone();
    for (int i = 0; i < maxFeaturePoints; i++) {
          if ((CurrentFramefeatures[i] == 0) || (CurrentFramefeatures[i] > 500))
            continue;

          cv::Point points[2];

          points[0] = cv::Point(cvRound(corners[0][i].x), cvRound(corners[0][i].y));
          points[1] = cv::Point(cvRound(corners[1][i].x), cvRound(corners[1][i].y));

          cv::line(OpticalFlowImage, points[0], points[1], CV_RGB(255, 0, 0), 1, 8, 0);
        }
}

void LKPyramid::DisplayFlow() {
    cv::imshow(windowNameCurr, currentFrame);
    cv::imshow(windowNamePrev, prevFrame);
    cv::imshow(windowNameOpticalFlow, OpticalFlowImage);
}
