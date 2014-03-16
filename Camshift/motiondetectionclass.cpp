#include "motiondetectionclass.h"
#include "lkpyramid.h"
#define THRESHOLD 50
#define ROWSTEPSIZE 2
#define COLSTEPSIZE 2

MotionDetectionClass::MotionDetectionClass()
{
    lkpyramidClass = LKPyramid::getInstance();
    element5    = cv::Mat(5,5, CV_8U, cv::Scalar(1));

}

void MotionDetectionClass::DisplayFlow() {
    lkpyramidClass->DisplayFlow();
}

void MotionDetectionClass::SetGridROI(int row, int col) {
     GridROI = cv::Rect(col, row, (2* COLSTEPSIZE), (2*ROWSTEPSIZE));
}

void MotionDetectionClass::SetGridPixelToValue(double pixelValue) {
    GridImage = motionMask(GridROI);
    GridImage.setTo(cv::Scalar(pixelValue));
    GridImage.copyTo(motionMask(GridROI));
}

void MotionDetectionClass::FillTheMotionMask() {

    if(motionMask.empty())
        return;

    for(int row = 0; row < currentFrame.rows - ROWSTEPSIZE; row=row+(2*ROWSTEPSIZE)) {
        for(int col = 0; col < currentFrame.cols - COLSTEPSIZE; col=col+(2*COLSTEPSIZE)) {
            SetGridROI(row, col);
            SetGridPixelToValue(motionMask.at<uchar>(row+ROWSTEPSIZE, col+COLSTEPSIZE));
            col++;
        }
        row++;
    }
}

cv::Point MotionDetectionClass::GetTheCentroid() {
    cv::Moments handmoment = cv::moments(handRegion, true);
    cv::Point centroidpoint(handmoment.m10/handmoment.m00, handmoment.m01/handmoment.m00);
    return centroidpoint;
}

void MotionDetectionClass::DrawCentroid(cv::Point centroidPoint) {
    cv::circle(currentFrame, centroidPoint, 10, cv::Scalar(255,0,0), 2, 8,0);
    cv::imshow("motion mask", motionMask);
}

void MotionDetectionClass::SeperateHand() {
    cv::normalize(motionMask, motionMask,0, 255, cv::NORM_MINMAX, CV_8UC1);
    cv::threshold(motionMask, handRegion, 200,255, CV_8UC1);
    cv::Point centroidPoint = GetTheCentroid();
    DrawCentroid(centroidPoint);
}

void MotionDetectionClass::SetImageWindowName() {
    lkpyramidClass->SetCurrentFrameWindow("current frame optical window");
    lkpyramidClass->SetPrevFrameWindow("previous frame optical flow window");
    lkpyramidClass->SetOpticalFlowWindow("optical flow window");
}

cv::Mat MotionDetectionClass::GetTheMotionMask(const cv::Mat inputFrame) {
    currentFrame = inputFrame.clone();
    SetImageWindowName();
    lkpyramidClass->CalculateOpticalFlow(currentFrame);
    motionMask = lkpyramidClass->GetTheMotionMask();
    if(motionMask.empty())
        return cv::Mat();
    FillTheMotionMask();
    SeperateHand();
    return motionMask;

    //cv::imshow("hand", motionMask);
}

