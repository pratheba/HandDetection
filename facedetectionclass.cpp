#include "facedetectionclass.h"

// remove hardcoded value and replace as argument passing
FaceDetectionClass::FaceDetectionClass():haarClassifier(new HaarCascaseClassifierClass("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/haarcascades/haarcascade_frontalface_default.xml")), isFramesReached(false),
    maskForCamshift(new MaskForCamshift())
{
   // ProcessFrameToDetectfacePtr = &(ProcessFrameToDetectface(cv::Mat&, cv::Mat&));
}


FaceDetectionClass::~FaceDetectionClass() {
    if(haarClassifier != NULL)  {   delete haarClassifier;      haarClassifier = NULL; }
    if(maskForCamshift != NULL) {   delete maskForCamshift;     maskForCamshift = NULL; }
}

void FaceDetectionClass::Initialize() {
    haarClassifier      =   new HaarCascaseClassifierClass("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/haarcascades/haarcascade_frontalface_default.xml");
    maskForCamshift     =   new MaskForCamshift();
    isFramesReached     =   false;
}


void FaceDetectionClass::DetectFace() {
    if(haarClassifier == NULL) {
        Initialize();
    }
}

cv::Point2d FaceDetectionClass::GetPositionOfFaceDetected() {
    return faceRectanglePoint;
}

cv::Size    FaceDetectionClass::GetsizeOffaceDetected() {
    return facerectiangleSize;
}

void FaceDetectionClass::SaveRegionOfInterestAsHSV(cv::Mat& outputFrame, cv::Rect_<int> ROIrectangle) {
    cv::Mat RegionOfInterestinHSV(outputFrame, ROIrectangle);
    //RegionOfInterestinHSV = cv::Mat(outputFrame, ROIrectangle);// (outputFrame, ROIrectangle);
    //cv::cvtColor(RegionOfInterestinHSV, RegionOfInterestinHSV, CV_BGR2HSV);
   // std::cout << ROIrectangle.x << "::" << ROIrectangle.y << "::" << RegionOfInterestinHSV.total() << std::endl;
    regionOfInterestinHSV.push_back(RegionOfInterestinHSV);
    //std::cout << regionOfInterestinHSV.size() << std::endl;
}

void FaceDetectionClass::ProcessFrameToDetectface(cv::Mat& inputFrame, cv::Mat& outputFrame) {
    haarClassifier->ProcessFrame(inputFrame, outputFrame);
    cv::Rect_<int> ROIrectangle = (haarClassifier->GetPositionAndScaleOfDetectedItem());
    SaveRegionOfInterestAsHSV(outputFrame, ROIrectangle);
    positionAndScale.push_back(ROIrectangle);
}

bool FaceDetectionClass::isRequiredNumberOfFramesreached() {
    return isFramesReached;
}


void FaceDetectionClass::AverageDetectedFace() {

     std::vector<cv::Mat>::iterator iter =   regionOfInterestinHSV.begin();

    if(isRequiredNumberOfFramesreached()) {
        for(; iter != regionOfInterestinHSV.end(); iter++)
            maskForCamshift->CalculateHistogramForImages(*iter);
    }

}

MinMaxHSVValue FaceDetectionClass::GetHSVmaskForFace() {
    AverageDetectedFace();
    return(maskForCamshift->GetMinMaxValueForSkinColor());
}

