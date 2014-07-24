#include "maskforcamshift.h"

MaskForCamshift::MaskForCamshift(): huerange({0, 180}), saturationrange({0, 256}), valuerange({0, 256}), uniform(true), accumulate(false),
    hueAccumulator({0}), saturationAccumulator({0}), valueAccumulator({0}), totalNumberOfimages(0), isMinMaxValueSet(false)
{

    //huerange = {0, 180};
    //saturationrange = {0, 256};
    //valueRange = {0, 256};

    hBins = 180;
    sBins = 256;
    vBins = 256;

    minHue = -1;//std::numeric_limits<int>::max();
    maxHue = 181;//std::numeric_limits<int>::min();

    minSat = -1;//std::numeric_limits<int>::max();
    maxSat = 257;//std::numeric_limits<int>::min();

    minValue = -1;//std::numeric_limits<int>::max();
    maxValue = 257;//std::numeric_limits<int>::min();


   // hueAccumulator = {0};
   // saturationAccumulator = {0};
   // valueAccumulator = {0};

    //histSize = {hbins, sBins, vBins};

    //hueRange    =   {hueRange};
    //saturationRange = {saturationrange};
    //valueRange = {valuerange};
}


MinMaxHSVValue MaskForCamshift::GetMinMaxValueForSkinColor() {
    if(!isMinMaxValueSet) {
        GetAverageforeachBin();
        isMinMaxValueSet = true;
    }
    return minMaxHSVvalue;
}

void MaskForCamshift::SetMinMaxHSVValue() {
    minMaxHSVvalue.minhue = std::min(minHue, maxHue);
    minMaxHSVvalue.maxhue = maxHue;
    minMaxHSVvalue.minSat = minSat;
    minMaxHSVvalue.maxSat = maxSat;
    minMaxHSVvalue.minValue = minValue;
    minMaxHSVvalue.maxvalue = maxValue;

}

void MaskForCamshift::CalculateMinMaxValueForSkinColor() {
    // Set up a threshold value for skin color.. say > 10

    int minH = std::numeric_limits<int>::max();
    int maxH = std::numeric_limits<int>::min();
    int minS = std::numeric_limits<int>::max();
    int maxS = std::numeric_limits<int>::min();
    int minV = std::numeric_limits<int>::max();
    int maxV = std::numeric_limits<int>::min();

    for(int i=0; i < hBins; i++) {
        /*
        if((hueAccumulator[i] < minH) && (hueAccumulator[i] >= 10)) {
            minH = hueAccumulator[i];
            minHue = i;//hueAccumulator[i];
        }
        if(hueAccumulator[i] > maxH) {
            maxH = hueAccumulator[i];
            maxHue = i;//hueAccumulator[i];
        }*/

        if((minHue < 0) && (hueAccumulator[i] >= 5))
            minHue = i;
        if((hueAccumulator[i] >= 5))
            maxHue = i;
    }

    for(int i =0; i < sBins; i++) {
        /*if((saturationAccumulator[i] < minS) && (saturationAccumulator[i] >= 10)) {
            minS = saturationAccumulator[i];
            minSat = i;//saturationAccumulator[i];
        }
        if(saturationAccumulator[i] > maxS) {
            maxS = saturationAccumulator[i];
            maxSat = i;
        }


        if((valueAccumulator[i] < minV) && (valueAccumulator[i] >= 10)) {
            minV = valueAccumulator[i];
            minValue = i;
        }
        if(valueAccumulator[i] > maxV) {
            maxV = valueAccumulator[i];
            maxValue = i;
        }*/

        if((minS < 0 ) && (saturationAccumulator[i] >= 5))
            minS = i;
        if((saturationAccumulator[i] >= 5))
            maxS = i;


        if((minV < 0) && (valueAccumulator[i] >= 5))
            minV = i;
        if((valueAccumulator[i] >= 5))
            maxV = i;

    }
}


void MaskForCamshift::GetAverageforeachBin() {
    if(totalNumberOfimages <= 0)
        return;

    for(int h=0; h < hBins; h++) {
        hueAccumulator[h] /= totalNumberOfimages;
    }

    for(int sv =0; sv < sBins; sv++) {
        saturationAccumulator[sv] /= totalNumberOfimages;
        valueAccumulator[sv]      /= totalNumberOfimages;
    }
    CalculateMinMaxValueForSkinColor();
    SetMinMaxHSVValue();

}

void MaskForCamshift::AccumulateHSVBins() {
    for(int h=0; h < hBins; h++)
        hueAccumulator[h] += (int)hueHistogram.data[h];

    for(int sv =0; sv < sBins; sv++) {
        saturationAccumulator[sv] += (int) saturationHistogram.data[sv];
        valueAccumulator[sv]      += (int) valueHistogram.data[sv];
    }
}

void MaskForCamshift::ComputeHistogram() {

    const float* hueRange = { huerange };
    const float* saturationRange = { saturationrange};
    const float* valueRange = { valuerange};
    // Compute the histograms:

    if(&HSVChannels[0] == 0)
        return;
    calcHist( &HSVChannels[0], 1, 0, cv::Mat(), hueHistogram, 1, &hBins, &hueRange, uniform, accumulate );
    calcHist( &HSVChannels[1], 1, 0, cv::Mat(), saturationHistogram, 1, &sBins, &saturationRange, uniform, accumulate );
    calcHist( &HSVChannels[2], 1, 0, cv::Mat(), valueHistogram, 1, &vBins, &valueRange, uniform, accumulate );

    AccumulateHSVBins();


    //delete hueRange;
    //delete const_cast<float*>(saturationRange);
    //delete const_cast<float*>(valueRange);

}


void MaskForCamshift::CalculateHistogramForImages(cv::Mat inputImage) {
    totalNumberOfimages++;
    //cv::Mat inputImage = cv::imread("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/images/lena.bmp");
    cv::cvtColor(inputImage, inputImage, CV_BGR2HSV);

    HSVChannels.clear();
    cv::split( inputImage, HSVChannels );
    ComputeHistogram();
}
