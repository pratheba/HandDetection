#ifndef MASKFORCAMSHIFT_H
#define MASKFORCAMSHIFT_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <limits>


struct MinMaxHSVValue {
    int minhue, maxhue;
    int minSat, maxSat;
    int minValue, maxvalue;
};


class MaskForCamshift
{
public:
    MaskForCamshift();
    void CalculateHistogramForImages(cv::Mat inputImage);
    MinMaxHSVValue GetMinMaxValueForSkinColor();

private:
    int     totalNumberOfimages;
    int     hBins,  sBins,  vBins;
    bool    uniform,    accumulate;
    bool    isMinMaxValueSet;
    float   huerange[2],    saturationrange[2], valuerange[2];
    int     minHue, maxHue, minSat, maxSat, minValue, maxValue;
    int     hueAccumulator[181],    saturationAccumulator[256], valueAccumulator[256];

    MinMaxHSVValue minMaxHSVvalue;
    std::vector<cv::Mat> HSVChannels;
    cv::Mat hueHistogram, saturationHistogram, valueHistogram;

    void ComputeHistogram();
    void AccumulateHSVBins();
    void GetAverageforeachBin();
    void CalculateMinMaxValueForSkinColor();
    void SetMinMaxHSVValue();
};

#endif // MASKFORCAMSHIFT_H
