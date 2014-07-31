#ifndef HOUGHTRANSFORMCLASS_H
#define HOUGHTRANSFORMCLASS_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

class HoughTransformClass
{
public:
    HoughTransformClass();
    void findCircles(cv::Mat inputSource);
};

#endif // HOUGHTRANSFORMCLASS_H
