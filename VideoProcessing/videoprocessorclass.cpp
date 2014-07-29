#include "videoprocessorclass.h"


VideoProcessorClass* VideoProcessorClass::videoProcessorClass = NULL;
int VideoProcessorClass::videoDeviceID = -1;

VideoProcessorClass* VideoProcessorClass::getInstance() {
    if(videoProcessorClass != NULL)
        return videoProcessorClass;
    else
        return(getInstance(0)); // default video device ID
}

VideoProcessorClass* VideoProcessorClass::getInstance(int videoDeviceID_) {
    if(videoProcessorClass == NULL) {
        videoDeviceID           = videoDeviceID_;
        videoProcessorClass      =   new VideoProcessorClass();//(videoDeviceID_);
    }
    return videoProcessorClass;
}

/*
VideoProcessorClass::videoProcessorClass(int videoDeviceID_): facedetectionClass(new FaceDetectionClass()), videoDeviceID(videoDeviceID_), numberOfProcessedFrame(0),frameToStopProcessing(0),
    isStopProcessing(false)
{ }*/

VideoProcessorClass::VideoProcessorClass(): facedetectionClass(new FaceDetectionClass()),frameToStopProcessing(0),  numberOfProcessedFrame(0), isStopProcessing(false) {
    //if(videoDeviceID)
}

void VideoProcessorClass::release() {
    if(videoProcessorClass != NULL) {
        //delete videoProcessorClass;
        videoProcessorClass = NULL;
    }
}

void VideoProcessorClass::Initialize() {
    //Default to videoDeviceId 0
    videoDeviceID           =   0;
    frameToStopProcessing   =   0;
    numberOfProcessedFrame  =   0;
    facedetectionClass      =   new FaceDetectionClass();
}

cv::Mat VideoProcessorClass::GetCurrentFrame() {
    cv::Mat currentFrameCopy;
    currentFrame.copyTo(currentFrameCopy);
    return currentFrameCopy;
}


bool VideoProcessorClass::IsVideoDeviceEnabled() {
    if(capture.isOpened())
        return true;
    return false;
}

void VideoProcessorClass::SetVideoCaptureInstance() {
    capture = cv::VideoCapture(videoDeviceID);
}
void VideoProcessorClass::SetVideoCaptureInstance(int videoDeviceID_) {
    capture = cv::VideoCapture(videoDeviceID_);
}

void VideoProcessorClass::displayInput(std::string windowNameInput_) {
    windowNameInput = windowNameInput_;
    cv::namedWindow(windowNameInput);
}

void VideoProcessorClass::displayOutput(std::string windownameOutput_) {
    windowNameOutput = windownameOutput_;
    cv::namedWindow(windowNameOutput);
}

void VideoProcessorClass::SetframeToStopProcessing(int frameToStop_) {
    frameToStopProcessing = frameToStop_;
}

void VideoProcessorClass::setFrameProcessor( void(*frameProcessingCallBack) (cv::Mat&, cv::Mat&)) {
    process = frameProcessingCallBack;
}

bool VideoProcessorClass::isToStopProcessing() {
    return isStopProcessing;
}

void VideoProcessorClass::stopProcessing() {
    isStopProcessing = true;
}

int VideoProcessorClass::getFrameNumber() {
    return (int)(capture.get(CV_CAP_PROP_POS_FRAMES));
}

void VideoProcessorClass::DisplayFrames() {
    if(windowNameInput.length() != 0)
        cv::imshow(windowNameInput, currentFrame);
    if(windowNameOutput.length() != 0)
        cv::imshow(windowNameOutput, outputFrame);

}

void VideoProcessorClass::DisplayFrames(cv::Mat& currentFrame, cv::Mat& outputFrame) {
    if(windowNameInput.length() != 0)
        cv::imshow(windowNameInput, currentFrame);
    if(windowNameOutput.length() != 0)
        cv::imshow(windowNameOutput, outputFrame);

}

void VideoProcessorClass::StopDisplayingFrames() {
    cv::destroyAllWindows();
}

void VideoProcessorClass::ChooseProcessorClass(int frameProcessor) {

    switch (frameProcessor) {
    case 1:
        //setFrameProcessor(facedetectionClass->ProcessFrameToDetectface);
        break;
    default:
        break;
    }

}


void VideoProcessorClass::ProcessVideoFrame() {
    if(!IsVideoDeviceEnabled()) {
        std::cout << "Video frame could not be read\n . Try re-initializing\n" << std::endl;
        return;
    }
    int currentframenumber = 0;


    while(!isToStopProcessing()) {

        capture.read(currentFrame);
        outputFrame = currentFrame.clone();
        facedetectionClass->ProcessFrameToDetectface(outputFrame);
        currentframenumber++;

        DisplayFrames();

        if(currentframenumber == frameToStopProcessing) {
            facedetectionClass->isFramesReached = true;
            stopProcessing();
            StopDisplayingFrames();
        }

        int c = cvWaitKey(10);
        if(c==27)  //ESC key
            break;

        currentFrame.release();
         outputFrame.release();
    }

}
