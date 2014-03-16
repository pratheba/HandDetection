#ifndef IMAGESEGMENTATION_H
#define IMAGESEGMENTATION_H

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#include "Camshift/videoprocessorclass.h"

class ImageSegmentation
{
public:
    ImageSegmentation();
    void SegmentByWaterShed(cv::Mat inputFrame);

private:

    void FindDistanceTransformation(cv::Mat& inputFrame);
     VideoProcessorClass* videoProcessorClass;

};

#endif // IMAGESEGMENTATION_H
