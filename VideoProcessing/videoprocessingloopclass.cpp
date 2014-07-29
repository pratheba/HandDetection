#include "videoprocessingloopclass.h"

//VideoProcessingLoopClass::VideoProcessingLoopClass(const cv::Mat& inputoutputFrame_) :
//    facedetectionClass(new FaceDetectionClass()), inputoutputFrame(inputoutputFrame_),
//    windowNameInput("input Frame"), windowNameOutput("output Frame")
//{

//}


//void VideoProcessingLoopClass::operator()(const cv::Range& range) {

//    for(int init = range.start; init < range.end; init++) {

//        facedetectionClass->ProcessFrameToDetectface(&inputoutputFrame[init]);

//        DisplayFrames(init);

//        int c = cvWaitKey(10);
//        if(c==27)  //ESC key
//            break;


//    }
//}


//void VideoProcessingLoopClass::DisplayFrames(int init) const{
//    //if(windowNameInput.length() != 0)
//      //  cv::imshow(windowNameInput, currentFrame);
//    if(windowNameOutput.length() != 0)
//        cv::imshow(windowNameOutput, inputoutputFrame[init]);

//}
