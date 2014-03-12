#ifndef VIDEOPROCESSORCLASS_H
#define VIDEOPROCESSORCLASS_H

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include "facedetectionclass.h"

class VideoProcessorClass
{
public:
    VideoProcessorClass();
    VideoProcessorClass(int videoDeviceID_);

    void setFrameProcessor( void(*frameProcessingCallBack) (cv::Mat&, cv::Mat&));
    void SetVideoCaptureInstance(int videoDeviceId_);
    void SetVideoCaptureInstance();
    void SetframeToStopProcessing(int frameToStop);
    void displayInput(std::string windowNameInput_);
    void displayOutput(std::string windownameOutput_);

    void ChooseProcessorClass(int frameProcessor);
    void ProcessVideoFrame();
    cv::Mat GetCurrentFrame() ;
    void Initialize();

private:
    int videoDeviceID;
    cv::Mat currentFrame;
    cv::Mat outputFrame;
    cv::VideoCapture capture;
    std::string windowNameInput;
    std::string windowNameOutput;
    long frameToStopProcessing;
    long numberOfProcessedFrame;
    bool isStopProcessing;

    void(*process) (cv::Mat&, cv::Mat&);
    bool IsVideoDeviceEnabled();
    bool isToStopProcessing();
    void stopProcessing();
    int getFrameNumber();
    void DisplayFrames();
    void DisplayFrames(cv::Mat& currentFrame, cv::Mat& outputFrame);


    FaceDetectionClass* facedetectionClass;

};

#endif // VIDEOPROCESSORCLASS_H
