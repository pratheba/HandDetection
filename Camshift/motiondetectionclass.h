#ifndef MOTIONDETECTIONCLASS_H
#define MOTIONDETECTIONCLASS_H

#include <opencv2/core/core.hpp>
#include "lkpyramid.h"

class MotionDetectionClass
{
public:
    MotionDetectionClass();
    cv::Mat GetTheMotionMask(const cv::Mat inputFrame);
    cv::Point GetTheCentroid();

private:

    cv::Mat motionMask;
    cv::Mat handRegion;
    cv::Mat element5;
    cv::Rect GridROI;
    cv::Mat GridImage;
    cv::Mat currentFrame;
    cv::Point centroidPoint;

    void DisplayFlow();
    void SetTheMotionMask();
    void FillTheMotionMask();
    void SeperateHand();
    void DrawCentroid(cv::Point centroidPoint);

    void SetGridROI(int row, int col);
    void SetGridPixelToValue(double pixelValue);
    void SetImageWindowName();
    void SetTheCentroidPoint();

    LKPyramid* lkpyramidClass;



};

#endif // MOTIONDETECTIONCLASS_H
