#ifndef CAMSHIFTPROCESSING_H
#define CAMSHIFTPROCESSING_H


#include "videoprocessorclass.h"
#include "maskforcamshift.h"
#include "motiondetectionclass.h"
#include "regionofinterest.h"
#include "FingerTipDetection/contourclass.h"

class CamshiftProcessing
{
public:
    CamshiftProcessing();

    void TrackRegionOfInterest();

private:
     VideoProcessorClass* videoProcessorClass;
     MotionDetectionClass* motionMaskClass;
     RegionOfInterest* ROIClass;
     ContourClass* contourClass;
     MinMaxHSVValue HSVmaskValue;

     cv::Mat motionMask;
     cv::Mat CurrentFrame;
     cv::Mat HSVFrame;
     cv::Mat HueFrame;
     cv::Mat maskToCurrentFrame;
     cv::Rect ROIFortracking;
     cv::Rect BoundingBoxForROIForTracking;
     cv::Mat ROI;
     cv::Mat ROIMask;
     cv::Mat HSVMask;
     cv::Point CentroidPoint;
     cv::Point avgCenterPoint;

     cv::Mat histogramImage;
     int histogramsize;
     float histogramranges[2];
     cv::Rect trackingWindow;
     cv::Mat backProjectImage;
     cv::Mat finalProbabilityMask;
     cv::Mat DrawingBoard;
     cv::RotatedRect prevtrackingBox;

     bool   IsFirstSelection;
     bool flowMode;
     bool backprojMode;
     bool drawingMode;
     bool IsAverageDistance;

     double averageCamshiftDistance;

     void GetColorProbabilityMask();
     void SetOpticalFlow();
     void SelectRegionOfInterest();
     void GetRegionOfInterest();
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
     void DrawEllipseForTrackingObject(cv::RotatedRect trackingBox);
     void SetupDrawingBoard() ;

     double GetAverageValueForDistance(cv::RotatedRect trackingBox);
     bool IsAverageDistanceCalculated(cv::RotatedRect trackingBox);
     void newLocationOfCenter(cv::RotatedRect& trackingBox);


};

#endif // CAMSHIFTPROCESSING_H
