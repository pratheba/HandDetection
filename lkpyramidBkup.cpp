#include "lkpyramid.h"

#define WINDOW_SIZE  10
#define MAXFEATURECOUNT  400
#define THRESHOLD 50
#define ROWSTEPSIZE 2
#define COLSTEPSIZE 2


LKPyramid* LKPyramid::lkPyramidClass = NULL;

LKPyramid* LKPyramid::getInstance() {
    if(lkPyramidClass == NULL) {
        lkPyramidClass = new LKPyramid();
    }
    return lkPyramidClass;
}

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

    cv::namedWindow("motion mask");
}

void SetStepSize(int rowStepSize, int colStepSize) {
    ROWSTEPSIZE = rowStepSize;
    COLSTEPSIZE = colStepSize;
}

void SetThreshold(int threshold) {
    THRESHOLD = threshold;
}

void LKPyramid::reset() {
    if(lkPyramidClass != NULL) {
        delete lkPyramidClass;
        lkPyramidClass = NULL;
    }
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

void LKPyramid::ConvertFramesToSameType() {
    if(currentFrame.channels() > 1)
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY, CV_8U);

    currentFrame.convertTo(currentFrame, CV_8U);
    prevFrame.convertTo(prevFrame, CV_8U);

}


void LKPyramid::CalculateOpticalFlow(cv::Mat inputFrame) {

    if(!prevFrame.empty()) {
        currentFrame = inputFrame.clone();
        ConvertFramesToSameType();
        PrevFramefeatures = GridFeatures;

        cv::calcOpticalFlowPyrLK(prevFrame, currentFrame,
                                 PrevFramefeatures, CurrentFramefeatures,
                                 statusOfFlowForFeature, error,
                                 windowSizeForTracking, 3, termcriteria);
    }
    else if(prevFrame.empty()) {
        currentFrame = inputFrame.clone();
        GetGridFeatures();
        PrevFramefeatures = GridFeatures;
        CurrentFramefeatures = GridFeatures;
    }

    OpticalFlowImage = inputFrame.clone();
    cv::swap(prevFrame, currentFrame);
    std::swap(PrevFramefeatures, CurrentFramefeatures);

    currentFrame.release();
}

void LKPyramid::DrawAndDisplay() {
    DrawTheFlow();
    DisplayFlow();
}

void LKPyramid::FillTheMotionMask() {

    static int iter = 0;
    if(motionMask.empty())
        return;
    cv::Mat element5(5,5, CV_8U, cv::Scalar(1));


    for(int row = 0; row < currentFrame.rows - ROWSTEPSIZE; row=row+(2*ROWSTEPSIZE)) {
        for(int col = 0; col < currentFrame.cols - COLSTEPSIZE; col=col+(2*COLSTEPSIZE)) {
            cv::Rect GridROI = cv::Rect(col, row, (2* COLSTEPSIZE), (2*ROWSTEPSIZE));
            cv::Mat GridImage = motionMask(GridROI);
            GridImage.setTo(cv::Scalar((double)(motionMask.at<uchar>(row+ROWSTEPSIZE, col+COLSTEPSIZE))));
            GridImage.copyTo(motionMask(GridROI));

            /*cv::Mat binaryImage = handRegion(GridROI);
            binaryImage.setTo(cv::Scalar((double)(handRegion.at<uchar>(row+ROWSTEPSIZE, col+COLSTEPSIZE))));
            binaryImage.copyTo(handRegion(GridROI));*/
            col++;
        }
        row++;
    }

    cv::normalize(motionMask, motionMask,0, 255, cv::NORM_MINMAX, CV_8UC1);
    cv::threshold(motionMask, handRegion, 200,255, CV_8UC1);


    /*cv::blur(handRegion, handRegion, cv::Size(10,10));
    cv::morphologyEx(handRegion, handRegion, cv::MORPH_CLOSE, element5);
    cv::normalize(handRegion, handRegion, 0, 255, cv::NORM_MINMAX, CV_8UC1);*/

    //cv::compare(motionMask, cv::Scalar(60), handRegion, cv::CMP_GE);
    cv::Moments m = cv::moments(handRegion, true);
    cv::Point p(m.m10/m.m00, m.m01/m.m00);
    cv::circle(currentFrame, p, 10, cv::Scalar(255,0,0), 2, 8,0);


    cv::imshow("motion mask", motionMask);

    if(iter < 10 ) {
        std::string frame = "/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/images/Output/frame";
        std::stringstream ss;
        ss << iter;
        frame.append(ss.str());
        frame.append(".jpg");
        cv::imwrite(frame, motionMask);
        iter++;
    }
}


cv::Mat LKPyramid::GetTheMotionMask() {
    cv::Mat motionmaskClone = motionMask.clone();
       return motionmaskClone;
}


void LKPyramid::SetTheMotionMask() {
    motionMask.release();
    motionMask = cv::Mat(currentFrame.rows, currentFrame.cols, CV_8U, cv::Scalar(0.0));
}


void LKPyramid::SetTheFlowVector(cv::Point2f PrevPt, cv::Point2f currPt) {
    for (int i = 0; i < PrevFramefeatures.size(); i++) {
        if ((statusOfFlowForFeature[i] == 0) || (int)error.data[i] > 550)
            continue;
        double flowVector = std::abs(PrevPt.x - currPt.x) + std::abs(PrevPt.y - currPt.y);
        motionMask.at<uchar>(PrevPt.y, PrevPt.x) =  (flowVector <= THRESHOLD ? flowVector : 0);
        //if(flowVector > THRESHOLD)
        //handRegion.at<uchar>(PrevPt.y, PrevPt.x) = 255;
    }
}

void LKPyramid::DrawTheFlow() {

    for (int i = 0; i < PrevFramefeatures.size(); i++) {
        if ((statusOfFlowForFeature[i] == 0) || (int)error.data[i] > 550)
            continue;

          cv::Point2f PrevPt = cv::Point2f(cvRound(PrevFramefeatures[i].x), cvRound(PrevFramefeatures[i].y));
          cv::Point2f currPt = cv::Point2f(cvRound(CurrentFramefeatures[i].x), cvRound(CurrentFramefeatures[i].y));
          double angle = GetAngleOfFlow(PrevPt, currPt);
          doubel hypotenuse = getLengthOfFlow(PrevPt, currPt);

          currPt.x = (int) (PrevPt.x - 3 * hypotenuse * cos(angle));
          currPt.y = (int) (PrevPt.y - 3 * hypotenuse * sin(angle));
          DrawTheArrow(PrevPt, currPt, angle);
        }
}
double LKPyramid::GetAngleOfFlow(cv::Point2f PrevPt, cv::Point2f currPt) {
    double angle;
    angle = atan2( (double) PrevPt.y - currPt.y, (double) PrevPt.x - currPt.x );
    return angle;
}
double LKPyramid::getLengthOfFlow(cv::Point2f PrevPt, cv::Point2f currPt) {
    double hypotenuse;
    hypotenuse = sqrt( std::pow((PrevPt.y - currPt.y),2) + std::pow((PrevPt.x - currPt.x),2) );
    return hypotenuse;
}
void LKPyramid::DrawTheArrow(cv::Point2f PrevPt, cv::Point2f currPt, double angle) {
    cv::line(OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0);

     PrevPt.x = (int) (currPt.x + 5 * cos(angle + (3.14) / 4));
     PrevPt.y = (int) (currPt.y + 5 * sin(angle + (3.14) / 4));
     cv::line( OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0 );
     PrevPt.x = (int) (currPt.x + 5 * cos(angle - (3.14) / 4));
     PrevPt.y = (int) (currPt.y + 5 * sin(angle - (3.14) / 4));
     cv::line( OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0 );
}

void LKPyramid::DisplayFlow() {
    cv::imshow(windowNameCurr, currentFrame);
    cv::imshow(windowNamePrev, prevFrame);
    cv::imshow(windowNameOpticalFlow, OpticalFlowImage);
}


/// UNused code for this , but to be used for regular LKPyramid algorithm
/*
void LKPyramid::Initialize(cv::Mat inputFrame) {
    if(prevFrame.empty()) {
        GetFeaturesfortracking();
        GetCornerSubpixelfortracking();
        prevFrame = inputFrame.clone();
        PrevFramefeatures = CurrentFramefeatures;
    }
}

void LKPyramid::GetGridFeatures() {
    int row = ROWSTEPSIZE -1;
    int col = COLSTEPSIZE -1;

    if(GridFeatures.empty()) {
        for(row ; row < currentFrame.rows -ROWSTEPSIZE; row=row+(2*ROWSTEPSIZE)) {
            row++;
            col = COLSTEPSIZE -1;

            for(col ; col < currentFrame.cols -ROWSTEPSIZE; col=col+(2*COLSTEPSIZE)) {
                col++;
                GridFeatures.push_back(cv::Point2f(col,row));
            }
        }
    }
}

void LKPyramid::GetFeaturesfortracking() {
    if(currentFrame.channels() > 1)
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY, CV_8U);

    cv::goodFeaturesToTrack(currentFrame, CurrentFramefeatures,
                            maxFeaturePoints, featureQualityLevel,
                            minDistanceBetweenFeatures, cv::Mat(), windowSizeForFeatureSelection,
                            useHarrisDetector, valueForHarrisDetector);

}

void LKPyramid::GetCornerSubpixelfortracking() {
    if(currentFrame.channels() > 1)
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY, CV_8U);
    cv::cornerSubPix(currentFrame, CurrentFramefeatures, windowSizeForTracking, cv::Size(-1, -1), termcriteria);
}
*/

