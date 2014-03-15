#include "lkpyramid.h"

#define WINDOW_SIZE  10
#define MAXFEATURECOUNT  400
#define THRESHOLD 50
#define ROWSTEPSIZE 4
#define COLSTEPSIZE 5


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
    int row = ROWSTEPSIZE -1;
    int col = COLSTEPSIZE -1;

    /*if(ROWSTEPSIZE%2 == 0)
        rowInit = (int)(ROWSTEPSIZE)/2;
    else
        rowInit = (int) (ROWSTEPSIZE -1) /2;
    if(COLSTEPSIZE%2 == 0)
        colInit = (int)(COLSTEPSIZE)/2;
    else
        colInit = (int)(COLSTEPSIZE -1) /2;*/

    if(GridFeatures.empty()) {
        for( row ; row < currentFrame.rows -ROWSTEPSIZE; row=row+(2*ROWSTEPSIZE)) {
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

void LKPyramid::CalculateOpticalFlow(cv::Mat inputFrame) {



    if(!prevFrame.empty()) {
        currentFrame = inputFrame.clone();
        //GetFeaturesfortracking();
        //GetCornerSubpixelfortracking();

        if(currentFrame.channels() > 1)
            cv::cvtColor(currentFrame, currentFrame, CV_BGR2GRAY, CV_8U);

        currentFrame.convertTo(currentFrame, CV_8U);
        prevFrame.convertTo(prevFrame, CV_8U);
        PrevFramefeatures = GridFeatures;

        cv::calcOpticalFlowPyrLK(prevFrame, currentFrame,
                                 PrevFramefeatures, CurrentFramefeatures,
                                 statusOfFlowForFeature, error,
                                 windowSizeForTracking, 3, termcriteria);
        DrawTheFlow();
        DisplayFlow();
    }
    else if(prevFrame.empty()) {
        currentFrame = inputFrame.clone();
        GetGridFeatures();
        Initialize(inputFrame);
        PrevFramefeatures = GridFeatures;
        CurrentFramefeatures = GridFeatures;
    }

    OpticalFlowImage = inputFrame.clone();
    cv::swap(prevFrame, currentFrame);
    std::swap(PrevFramefeatures, CurrentFramefeatures);

    currentFrame.release();

}



void LKPyramid::FillTheMotionMask() {
    /*if(GridFeatures.empty()) {
        for( row ; row < currentFrame.rows -ROWSTEPSIZE; row=row+ROWSTEPSIZE) {
            row++;
            col = COLSTEPSIZE -1;
            for(col ; col < currentFrame.cols -ROWSTEPSIZE; col=col+COLSTEPSIZE) {
                col++;
                GridFeatures.push_back(cv::Point2f(col,row));
            }
        }
    }*/

    if(motionMask.empty())
        return;

    for(int row = 0; row < currentFrame.rows - ROWSTEPSIZE; row=row+(2*ROWSTEPSIZE)) {
        for(int col = 0; col < currentFrame.cols - COLSTEPSIZE; col=col+(2*COLSTEPSIZE)) {
            cv::Rect GridROI = cv::Rect(col, row, (2* COLSTEPSIZE), (2*ROWSTEPSIZE));
            cv::Mat GridImage = motionMask(GridROI);
            GridImage.setTo(cv::Scalar((double)(motionMask.at<uchar>(row+ROWSTEPSIZE, col+COLSTEPSIZE))));

            //std::cout << GridImage.rows << "::" << GridImage.cols << std::endl;
            //cv::cvtColor(GridImage, GridImage, CV_BGR2GRAY);//(double)motionMask.at<uchar>(col+COLSTEPSIZE, row+ROWSTEPSIZE), CV_8U);
            GridImage.copyTo(motionMask(GridROI));
            col++;
        }
        row++;
    }

    cv::normalize(motionMask, motionMask,0, 255, cv::NORM_MINMAX, CV_8UC1);

    cv::imshow("motion mask", motionMask);
}

cv::Mat LKPyramid::GetTheMotionMask() {
    //FillTheMotionMask();
    cv::Mat motionmaskClone = motionMask.clone();
       return motionmaskClone;
}

void LKPyramid::SetTheMotionMask() {
    motionMask.release();
    motionMask = cv::Mat(currentFrame.rows, currentFrame.cols, CV_8U, cv::Scalar(0.0));
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
          motionMask.at<uchar>(PrevPt.y, PrevPt.x) =  (flowVector <= THRESHOLD ? flowVector : 0);

          std::cout << (double)motionMask.at<uchar>(PrevPt.y, PrevPt.x) << std::endl;
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
    FillTheMotionMask();
}

void LKPyramid::DisplayFlow() {
    cv::imshow(windowNameCurr, currentFrame);
    cv::imshow(windowNamePrev, prevFrame);
    cv::imshow(windowNameOpticalFlow, OpticalFlowImage);
}
