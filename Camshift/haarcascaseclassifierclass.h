#ifndef HAARCASCASECLASSIFIERCLASS_H
#define HAARCASCASECLASSIFIERCLASS_H

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <iostream>


class HaarCascaseClassifierClass
{
public:
    HaarCascaseClassifierClass();
    HaarCascaseClassifierClass(std::string cascadeXML_);
    ~HaarCascaseClassifierClass();

    void ProcessFrame(cv::Mat& inputFrame, cv::Mat& outputFrame);
    cv::Rect_<int> GetPositionAndScaleOfDetectedItem();
    void reset();

private:
    cv::Mat currentFrame;
    cv::Mat processedFrame;
    std::string cascadeXML;
    cv::CascadeClassifier haarCascadeClassifier;
    cv::Rect_<int> posAndScale;

    std::vector< cv::Rect_<int> > itemsDetected;
    std::vector< cv::Rect_<int> >::iterator iter;
    std::vector< cv::Rect_<int> >::iterator iterEnd;

    void loadCascadeClassifier();
    void loadDetectedPointandWindowSize(cv::Mat& frame);

};

#endif // HAARCASCASECLASSIFIERCLASS_H
