#ifndef CONTOURCLASS_H
#define CONTOURCLASS_H


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Camshift/videoprocessorclass.h>

class ContourClass
{
public:
    ContourClass();
    ContourClass(cv::Mat contourImage);
    void FindContourImage(cv::Mat contourImage);
    void ShowContourImage();
private:
    cv::Mat contourImage;
    cv::Mat     contourInverse;
    std::vector<std::vector<cv::Point> > contourPoints;
    bool IsImageAvailableForCountourDetection();

    VideoProcessorClass* videoProcessorClass;

};

#endif // CONTOURCLASS_H
