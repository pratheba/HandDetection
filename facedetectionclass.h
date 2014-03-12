#ifndef FACEDETECTIONCLASS_H
#define FACEDETECTIONCLASS_H

#include "haarcascaseclassifierclass.h"
class FaceDetectionClass
{
public:
    FaceDetectionClass();
    void DetectFace();
    cv::Point2d GetPositionOfFaceDetected();
    cv::Size    GetsizeOffaceDetected();
    void ProcessFrameToDetectface(cv::Mat& inputFrame, cv::Mat& outputFrame);
    void FaceDetectionClass::GetHSVmaskForFace();

private:
    bool hasFace;
    cv::Point2d faceRectanglePoint;
    cv::Size    facerectiangleSize;
    std::vector <PositionAndScale> positionAndScale;
    HaarCascaseClassifierClass* haarClassifier;

    void Initialize();
};

#endif // FACEDETECTIONCLASS_H
