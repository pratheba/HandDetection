#ifndef LKPYRAMID_H
#define LKPYRAMID_H

#include <opencv2/video/tracking.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cmath>

class LKPyramid
{
public:
    LKPyramid();
    void CalculateOpticalFlow(cv::Mat currentFrame_);
    void SetCurrentFrameWindow(std::string windowNameCurr_);
    void SetPrevFrameWindow(std::string windowNamePrev_);
    void SetOpticalFlowWindow(std::string windowNameOpticalFlow_);
    cv::Mat GetTheMotionMask();


private:
    cv::Mat motionMask;
    cv::Mat currentFrame;
    cv::Mat prevFrame;
    cv::Mat OpticalFlowImage;
    cv::Mat GridMask;
    std::vector<cv::Point2f>    CurrentFramefeatures ;
    std::vector<cv::Point2f>    PrevFramefeatures ;
    std::vector<cv::Point2f>    GridFeatures;

    int     maxFeaturePoints;
    double  featureQualityLevel;
    double  minDistanceBetweenFeatures;
    bool    useHarrisDetector;
    double  valueForHarrisDetector;
    int     windowSizeForFeatureSelection;
    cv::Size windowSizeForTracking;
    cv::TermCriteria termcriteria;
    std::vector<uchar> statusOfFlowForFeature;
    cv::Mat error;

    std::string windowNamePrev;
    std::string windowNameCurr;
    std::string windowNameOpticalFlow;

    void GetFeaturesfortracking();
    void GetCornerSubpixelfortracking();
    void Initialize(cv::Mat currentFrame_);
    void DrawTheFlow();
    void DisplayFlow();
    void SetTheMotionMask();
    void GetGridFeatures();

    double threshold;


};

#endif // LKPYRAMID_H
