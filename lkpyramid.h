#ifndef LKPYRAMID_H
#define LKPYRAMID_H


#include <opencv2/video/tracking.hpp>
#include <opencv2/video/video.hpp>

class LKPyramid
{
public:
    LKPyramid();
    void CalculateOpticalFlow(cv::Mat currentFrame_);
    void SetCurrentFrameWindow(std::string windowNameCurr_);
    void SetPrevFrameWindow(std::string windowNamePrev_);
    void SetOpticalFlowWindow(std::string windowNameOpticalFlow_);


private:
    cv::Mat currentFrame;
    cv::Mat prevFrame;
    cv::Mat OpticalFlowImage;
    std::vector<cv::Point2f>    CurrentFramefeatures ;
    std::vector<cv::Point2f>    PrevFramefeatures ;

    int     maxFeaturePoints;
    double  featureQualityLevel;
    double  minDistanceBetweenFeatures;
    bool    useHarrisDetector;
    double  valueForHarrisDetector;
    int     windowSizeForFeatureSelection;
    cv::Size windowSizeForTracking;
    cv::TermCriteria termcriteria;
    std::vector<uchar> statusOfFlowForFeature;
    std::vector<float> error;

    std::string windowNamePrev;
    std::string windowNameCurr;
    std::string windowNameOpticalFlow;

    void GetFeaturesfortracking();
    void GetCornerSubpixelfortracking();
    void Initialize(cv::Mat& currentFrame_);
    void Initialize();
    void DrawTheFlow();
    void DisplayFlow();


};

#endif // LKPYRAMID_H
