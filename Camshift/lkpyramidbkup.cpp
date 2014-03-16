#include "lkpyramid.h"

#define WINDOW_SIZE  10
#define MAXFEATURECOUNT  500


LKPyramid::LKPyramid()
{
    prevFrame   = cv::Mat();
    currentFrame = cv::Mat();
    GridMask = cv::Mat();
    CurrentFramefeatures.reserve(500);
    PrevFramefeatures.reserve(500);
    GridFeatures.reserve(500);

    maxFeaturePoints = 500;
    featureQualityLevel = 0.01;
    minDistanceBetweenFeatures = 3;
    useHarrisDetector = false;
    valueForHarrisDetector = 0.04;
    windowSizeForFeatureSelection = 3;
    threshold = 40;

    windowSizeForTracking = cv::Size(10,10);
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

void LKPyramid::GetGridFeatures() {
    if(GridFeatures.empty()) {
        for(int row = 1; row < currentFrame.rows -5; row=row+10)
            for(int col = 1; col < currentFrame.cols -5; col=col+10) {
                GridFeatures.push_back(cv::Point2f(row,col));
            }
    }
}

void LKPyramid::GetFeaturesfortracking() {
    if(currentFrame.channels() > 1)
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY,CV_8U);

    cv::goodFeaturesToTrack(currentFrame, CurrentFramefeatures,
                            maxFeaturePoints, featureQualityLevel,
                            minDistanceBetweenFeatures, cv::Mat(), windowSizeForFeatureSelection,
                            useHarrisDetector, valueForHarrisDetector);

}


void LKPyramid::GetCornerSubpixelfortracking() {
    if(currentFrame.channels() > 1)
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY,CV_8U);
    cv::cornerSubPix(currentFrame, CurrentFramefeatures, windowSizeForTracking, cv::Size(-1, -1), termcriteria);
}

void LKPyramid::CalculateOpticalFlow(cv::Mat inputFrame) {
   currentFrame = inputFrame.clone();
    if(currentFrame.channels() > 1)
        cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY, CV_8U);
    
    if(!prevFrame.empty()) {
        currentFrame = inputFrame.clone();
        GetFeaturesfortracking();
        GetCornerSubpixelfortracking();

        currentFrame.convertTo(currentFrame, CV_8U);
        prevFrame.convertTo(currentFrame, CV_8U);
        cv::calcOpticalFlowPyrLK(prevFrame, currentFrame,
                                 PrevFramefeatures, CurrentFramefeatures,
                                 statusOfFlowForFeature, error,
                                 windowSizeForTracking, 3, termcriteria);
        DrawTheFlow();
        DisplayFlow();
    }
    else if(prevFrame.empty()) {

        currentFrame = inputFrame.clone();
        if(currentFrame.channels() > 1)
            cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY, CV_8U);

         //GetGridFeatures();
         //PrevFramefeatures = GridFeatures;
         //CurrentFramefeatures = GridFeatures;

        Initialize(inputFrame);
        //PrevFramefeatures =
    }

    OpticalFlowImage = inputFrame.clone();
    cv::swap(prevFrame, currentFrame);
    std::swap(PrevFramefeatures, CurrentFramefeatures);

    currentFrame.release();

}

cv::Mat LKPyramid::GetTheMotionMask() {
    cv::Mat motionmaskClone = motionMask.clone();
    return motionmaskClone;
}

void LKPyramid::SetTheMotionMask() {
    motionMask.release();
    motionMask = cv::Mat(currentFrame.rows, currentFrame.cols, CV_8U, cv::Scalar(0.0));

    //motionMask = cv::Scalar(1,1,1);
}

void LKPyramid::DrawTheFlow() {
    //SetTheMotionMask();
    //std::cout << motionMaskMat.rows << "::" << motionMaskMat.cols << std::endl;

    for (int i = 0; i < PrevFramefeatures.size(); i++) {
   //  for (int i = 0; i < GridFeatures.size(); i++) {
        if ((statusOfFlowForFeature[i] == 0) || (int)error.data[i] > 550)
            continue;

          cv::Point2f PrevPt, currPt;

          //PrevPt = cv::Point2f(cvRound(PrevFramefeatures[i].x), cvRound(PrevFramefeatures[i].y));
          //currPt = cv::Point2f(cvRound(CurrentFramefeatures[i].x), cvRound(CurrentFramefeatures[i].y));

          PrevPt.x = cvRound(PrevFramefeatures[i].x);
          PrevPt.y = cvRound(PrevFramefeatures[i].y);
          currPt.x = cvRound(CurrentFramefeatures[i].x);
          currPt.y = cvRound(CurrentFramefeatures[i].y);

          /*double flowVector = std::abs(PrevPt.x - currPt.x) + std::abs(PrevPt.y - currPt.y);
          if(flowVector <= threshold) {
              // set all the pixels in the grid to be flowvector;
              motionMask.at<cv::Vec3b>(PrevPt.y, PrevPt.x)[0] = flowVector;//0.0;
              motionMask.at<cv::Vec3b>(PrevPt.y, PrevPt.x)[1] = flowVector;
              motionMask.at<cv::Vec3b>(PrevPt.y, PrevPt.x)[2] = flowVector;

          } if(flowVector > threshold){
              motionMask.at<cv::Vec3b>(PrevPt.y, PrevPt.x)[0] = 0.0;//255;//flowVector;//0.0;
              motionMask.at<cv::Vec3b>(PrevPt.y, PrevPt.x)[1] = 0.0;//255;//flowVector;//0.0;
              motionMask.at<cv::Vec3b>(PrevPt.y, PrevPt.x)[2] = 0.0;//255;//flowVector;//0.0;
          }*/

          //std::cout << PrevFramefeatures[i].x << "::" << PrevFramefeatures[i].y << std::endl;

         // if(flowVector <= threshold)
           //   cv::circle(OpticalFlowImage, PrevPt, 2, CV_RGB(0,255,0), 1, 8, CV_AA);

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

