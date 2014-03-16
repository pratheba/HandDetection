#ifndef CAMSHIFTPROCESSING_H
#define CAMSHIFTPROCESSING_H


#include "videoprocessorclass.h"
#include "maskforcamshift.h"
#include "motiondetectionclass.h"
#include "regionofinterest.h"
#include "FingerTipDetection/imagesegmentation.h"

class CamshiftProcessing
{
public:
    CamshiftProcessing();

    void TrackRegionOfInterest();

private:
     VideoProcessorClass* videoProcessorClass;
     MotionDetectionClass* motionMaskClass;
     RegionOfInterest* ROIClass;
     MinMaxHSVValue HSVmaskValue;

     cv::Mat motionMask;
     cv::Mat CurrentFrame;
     cv::Mat HSVFrame;
     cv::Mat HueFrame;
     cv::Mat maskToCurrentFrame;
     cv::Rect ROIFortracking;
     cv::Mat ROI;
     cv::Mat ROIMask;
     cv::Mat HSVMask;

     cv::Mat histogramImage;
     int histogramsize;
     float histogramranges[2];
     cv::Rect trackingWindow;
     cv::Mat backProjectImage;
     cv::Mat finalProbabilityMask;
     bool   IsFirstSelection;

     double minDisplacement;
     double maxDisplacement;
     cv::Point minDisplacementLocation;
     cv::Point maxDisplacementLocation;

     bool flowMode;
     bool backprojMode;

     void GetColorProbabilityMask();
     void SetOpticalFlow();
     void SelectRegionOfInterest();
     void InitializeCamshift();
     void GetFinalProbabilityMask();
     void StartCamshift();
     void ActionOnKeyPress(int key);
     void DrawTrackingObject(cv::RotatedRect trackingBox);
     void ConvertBGRImageToHSV();
     void SetHSVColorProbabilityMask() ;
     void GetHueFrameFromHSVFrame();
     void CalculateHistogramForRegionOfInterest();
     void CalculateBackProjection();
     void ApplyFinalProbabilityMask();


};

#endif // CAMSHIFTPROCESSING_H
