#ifndef MOTIONDETECTIONCLASS_H
#define MOTIONDETECTIONCLASS_H

#include <opencv2/core/core.hpp>
#include "lkpyramid.h"

class MotionDetectionClass
{
public:
    MotionDetectionClass();
    cv::Mat GetTheMotionMask(const cv::Mat inputFrame);


private:

    cv::Mat motionMask;
    cv::Mat handRegion;
    cv::Mat element5;
    cv::Rect GridROI;
    cv::Mat GridImage;
    cv::Mat currentFrame;

    void DisplayFlow();
    void SetTheMotionMask();
    void FillTheMotionMask();
    void SeperateHand();
    void DrawCentroid(cv::Point centroidPoint);
    cv::Point GetTheCentroid() ;
    void SetGridROI(int row, int col);
    void SetGridPixelToValue(double pixelValue);
    void SetImageWindowName();

    LKPyramid* lkpyramidClass;



};

#endif // MOTIONDETECTIONCLASS_H
