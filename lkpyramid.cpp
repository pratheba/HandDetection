#include "lkpyramid.h"

#define WINDOW_SIZE  10
#define MAXFEATURECOUNT  400
#define THRESHOLD 10


LKPyramid::LKPyramid()
{
    prevFrame   = cv::Mat();
    currentFrame = cv::Mat();
    CurrentFramefeatures.reserve(MAXFEATURECOUNT);
    PrevFramefeatures.reserve(MAXFEATURECOUNT);

    maxFeaturePoints = MAXFEATURECOUNT;
    featureQualityLevel = 0.01;
    minDistanceBetweenFeatures = 10;
    useHarrisDetector = false;
    valueForHarrisDetector = 0.04;
    windowSizeForFeatureSelection = 3;

    windowSizeForTracking = cv::Size(WINDOW_SIZE,WINDOW_SIZE);
    termcriteria = cv::TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);

    //videoProcessorClass = VideoProcessorClass.getInstance();

}

void LKPyramid::SetCurrentFrameWindow(std::string windowNameCurr_) {
    windowNameCurr = windowNameCurr_;
    cv::namedWindow(windowNameCurr);
}

void LKPyramid::SetPrevFrameWindow(std::string windowNamePrev_) {
    windowNamePrev = windowNamePrev_;
    cv::namedWindow(windowNamePrev);
}

void LKPyramid::SetOpticalFlowWindow(std::string windowNameOpticalFlow_) {
    windowNameOpticalFlow = windowNameOpticalFlow_;
    cv::namedWindow(windowNameOpticalFlow);
}


void LKPyramid::Initialize(cv::Mat inputFrame) {
    if(prevFrame.empty()) {
        GetFeaturesfortracking();
        GetCornerSubpixelfortracking();
        prevFrame = inputFrame.clone();
        PrevFramefeatures = CurrentFramefeatures;
    }
}

void LKPyramid::GetFeaturesfortracking() {
    if(currentFrame.channels() > 1)
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY);

    cv::goodFeaturesToTrack(currentFrame, CurrentFramefeatures,
                            maxFeaturePoints, featureQualityLevel,
                            minDistanceBetweenFeatures, cv::Mat(), windowSizeForFeatureSelection,
                            useHarrisDetector, valueForHarrisDetector);

}


void LKPyramid::GetCornerSubpixelfortracking() {
    if(currentFrame.channels() > 1)
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY);
    cv::cornerSubPix(currentFrame, CurrentFramefeatures, windowSizeForTracking, cv::Size(-1, -1), termcriteria);
}

void LKPyramid::CalculateOpticalFlow(cv::Mat inputFrame) {
    
    if(!prevFrame.empty()) {
        currentFrame = inputFrame.clone();
        GetFeaturesfortracking();
        GetCornerSubpixelfortracking();

        cv::calcOpticalFlowPyrLK(prevFrame, currentFrame,
                                 PrevFramefeatures, CurrentFramefeatures,
                                 statusOfFlowForFeature, error,
                                 windowSizeForTracking, 3, termcriteria);
        DrawTheFlow();
        DisplayFlow();
    }
    else if(prevFrame.empty()) {
        currentFrame = inputFrame.clone();
        Initialize(inputFrame);
    }

    OpticalFlowImage = inputFrame.clone();
    cv::swap(prevFrame, currentFrame);
    std::swap(PrevFramefeatures, CurrentFramefeatures);

    currentFrame.release();

}

cv::Mat LKPyramid::GetTheMotionMask() {

}

void LKPyramid::SetTheMotionMask() {
    motionMask = cv::Mat(currentFrame.rows, currentFrame.cols, currentFrame.type());
    motionMask = cv::Scalar(0);
}

void LKPyramid::DrawTheFlow() {
    SetTheMotionMask();

    for (int i = 0; i < PrevFramefeatures.size(); i++) {
        if ((statusOfFlowForFeature[i] == 0) || (int)error.data[i] > 550)
            continue;

          cv::Point2f PrevPt, currPt;

          PrevPt = cv::Point2f(cvRound(PrevFramefeatures[i].x), cvRound(PrevFramefeatures[i].y));
          currPt = cv::Point2f(cvRound(CurrentFramefeatures[i].x), cvRound(CurrentFramefeatures[i].y));

          double flowVector = std::abs(PrevPt.x - currPt.x) + std::abs(PrevPt.y - currPt.y);
          motionMask.at<uchar>(PrevPt.x, PrevPt.y) =  (flowVector >= THRESHOLD ? flowVector : 0);
          if(flowVector >= THRESHOLD)
              cv::circle(OpticalFlowImage, PrevPt, 2, CV_RGB(0,255,0), 1, 8, CV_AA);

          double angle;
          angle = atan2( (double) PrevPt.y - currPt.y, (double) PrevPt.x - currPt.x );
          double hypotenuse;
          hypotenuse = sqrt( std::pow((PrevPt.y - currPt.y),2) + std::pow((PrevPt.x - currPt.x),2) );

          currPt.x = (int) (PrevPt.x - 3 * hypotenuse * cos(angle));
          currPt.y = (int) (PrevPt.y - 3 * hypotenuse * sin(angle));


          cv::line(OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0);

           PrevPt.x = (int) (currPt.x + 5 * cos(angle + (3.14) / 4));
           PrevPt.y = (int) (currPt.y + 5 * sin(angle + (3.14) / 4));
           cv::line( OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0 );
           PrevPt.x = (int) (currPt.x + 5 * cos(angle - (3.14) / 4));
           PrevPt.y = (int) (currPt.y + 5 * sin(angle - (3.14) / 4));
           cv::line( OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0 );

        }
}

void LKPyramid::DisplayFlow() {
    cv::imshow(windowNameCurr, currentFrame);
    cv::imshow(windowNamePrev, prevFrame);
    cv::imshow(windowNameOpticalFlow, OpticalFlowImage);
}
