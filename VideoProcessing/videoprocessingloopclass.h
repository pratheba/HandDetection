#ifndef VIDEOPROCESSINGLOOPCLASS_H
#define VIDEOPROCESSINGLOOPCLASS_H

#include "opencv2/core/core.hpp"
#include "FaceDetection/facedetectionclass.h"

// This class is dedicated for parallel loop processing only

class VideoProcessingLoopClass : public cv::ParallelLoopBody
{
private:

    const cv::Mat& inputoutputFrame;

    FaceDetectionClass* facedetectionClass;

    std::string windowNameInput;
    std::string windowNameOutput;

    //void DisplayFrames(int init) const;


public:
//    VideoProcessingLoopClass(const cv::Mat& inputoutputFrame);

//    virtual void operator()(const cv::Range& range) ;


};

#endif // VIDEOPROCESSINGLOOPCLASS_H
