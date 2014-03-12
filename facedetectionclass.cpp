#include "facedetectionclass.h"

// remove hardcoded value and replace as argument passing
FaceDetectionClass::FaceDetectionClass():haarClassifier(new HaarCascaseClassifierClass("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/haarcascades/haarcascade_frontalface_default.xml"))
{
   // ProcessFrameToDetectfacePtr = &(ProcessFrameToDetectface(cv::Mat&, cv::Mat&));
}


void FaceDetectionClass::Initialize() {
    haarClassifier = new HaarCascaseClassifierClass("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/haarcascades/haarcascade_frontalface_default.xml");
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

void FaceDetectionClass::ProcessFrameToDetectface(cv::Mat& inputFrame, cv::Mat& outputFrame) {
    haarClassifier->ProcessFrame(inputFrame, outputFrame);
    positionAndScale    =   haarClassifier->GetPositionAndScaleOfDetectedItem();
}

void FaceDetectionClass::GetHSVmaskForFace() {

}

