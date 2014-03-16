#ifndef CAMSHIFTPROCESSING_H
#define CAMSHIFTPROCESSING_H


#include "videoprocessorclass.h"
#include "maskforcamshift.h"
#include "motiondetectionclass.h"

class CamshiftProcessing
{
public:
    CamshiftProcessing();

    void TrackRegionOfInterest();
    //FaceDetectionClass* faceDetection;

private:
     VideoProcessorClass* videoProcessorClass;
     //LKPyramid*             lkPyramidClass;
     MotionDetectionClass* motionMaskClass;
     MinMaxHSVValue HSVmaskValue;
     cv::Mat motionMask;
     cv::Mat CurrentFrame;
     cv::Mat HSVFrame;
     cv::Mat HueFrame;
     cv::Mat maskToCurrentFrame;

     double minDisplacement;
     double maxDisplacement;
     cv::Point minDisplacementLocation;
     cv::Point maxDisplacementLocation;

     void GetColorProbabilityMask();
     cv::Mat GetOpticalFlow();
     void SelectRegionOfInterest();
     bool IsBothMatrixOfSameSize(cv::Mat& inputMat1, cv::Mat& inputMat2);
     cv::Point2i GetStepSizeForROIselection(int rowWidth, int colWidth);
     cv::Rect GetROIForProcessing(int row, int col, int height, int width);

};

#endif // CAMSHIFTPROCESSING_H
