#ifndef FACEDETECTIONCLASS_H
#define FACEDETECTIONCLASS_H

#include "haarcascaseclassifierclass.h"
#include "maskforcamshift.h"

class FaceDetectionClass
{
public:
    FaceDetectionClass();
    ~FaceDetectionClass();

    void DetectFace();
    cv::Point2d GetPositionOfFaceDetected();
    cv::Size    GetsizeOffaceDetected();
    void ProcessFrameToDetectface(cv::Mat& inputFrame, cv::Mat& outputFrame);
    MinMaxHSVValue GetHSVmaskForFace();

    bool isFramesReached;

    //
    std::vector <cv::Mat>           regionOfInterestinHSV;

private:
    bool            hasFace;
    cv::Point2d     faceRectanglePoint;
    cv::Size        facerectiangleSize;
    std::vector <cv::Rect_<int> >   positionAndScale;
    //std::vector <cv::Mat>           regionOfInterestinHSV;


    void    Initialize();
    void    SaveRegionOfInterestAsHSV(cv::Mat& outputFrame, cv::Rect_<int> ROIrectangle);
    bool    isRequiredNumberOfFramesreached();
    void    AverageDetectedFace();

    HaarCascaseClassifierClass*     haarClassifier;
    MaskForCamshift*                maskForCamshift;


};

#endif // FACEDETECTIONCLASS_H
