#ifndef LKLOOPPARALLELLCLASS_H
#define LKLOOPPARALLELLCLASS_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "tbb/parallel_for_each.h"
#include "tbb/tick_count.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"


class LKLoopParallellClass
{
private:
    const std::vector<cv::Point2f>&    CurrentFramefeatures ;
    const std::vector<cv::Point2f>&    PrevFramefeatures ;
    cv::Mat& OpticalFlowImage;
    cv::Mat& error;
    const std::vector<uchar>& statusOfFlowForFeature;

    double GetAngleOfFlow(cv::Point2f PrevPt, cv::Point2f currPt) const;
    double getLengthOfFlow(cv::Point2f PrevPt, cv::Point2f currPt) const;
    void DrawTheArrow(cv::Point2f PrevPt, cv::Point2f currPt, double angle) const;

public:

    void operator()(const tbb::blocked_range<int> &range) const;

    LKLoopParallellClass(cv::Mat& OpticalFlowImage_,cv::Mat& error_,std::vector<cv::Point2f>& currentFeatures, std::vector<cv::Point2f>& prevFeatures, std::vector<uchar>& statusOfFlowForFeature_);
   // LKLoopParallellClass(LKLoopParallellClass& lkloopclass);
    ~LKLoopParallellClass();

};

#endif // LKLOOPPARALLELLCLASS_H
