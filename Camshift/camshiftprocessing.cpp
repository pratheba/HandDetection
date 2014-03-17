#include "camshiftprocessing.h"

#define CENTROIDTHRESHOLD 200

CamshiftProcessing::CamshiftProcessing():histogramranges({0,180}), backprojMode(false), flowMode(false), drawingMode(false)
{
    videoProcessorClass = VideoProcessorClass::getInstance();
    motionMaskClass = new MotionDetectionClass();
    ROIClass = new RegionOfInterest();
    contourClass = new ContourClass();
    motionMask = cv::Mat();
    cv::namedWindow("ROI",0);
    IsFirstSelection = true;
    histogramsize = 180;
    averageCamshiftDistance = 0;
    IsAverageDistance = false;

    DrawingBoard = cv::Mat();
}

void CamshiftProcessing::GetColorProbabilityMask() {
    if(videoProcessorClass->facedetectionClass != NULL) {
        HSVmaskValue = videoProcessorClass->facedetectionClass->GetHSVmaskForFace();
    }
    else {float* phranges;

        std::cout << "No processing has been done to get the HSV value"  << std::endl;
        return;
    }
}

void CamshiftProcessing::SetOpticalFlow() {
    if(motionMask.empty())
        motionMask = cv::Mat(CurrentFrame.rows, CurrentFrame.cols, CV_8U, cv::Scalar(1.0));
    motionMask = cv::Scalar(1.0);
    motionMask = motionMaskClass->GetTheMotionMask(CurrentFrame.clone());
    CentroidPoint = motionMaskClass->GetTheCentroid();
    //if(!motionMask.empty())
      //  motionMaskClass->DisplayFlow();
}

void CamshiftProcessing::CalculateBackProjection() {
    const float* phranges = histogramranges;
    cv::calcBackProject(&HueFrame, 1, 0, histogramImage, backProjectImage, &phranges);
    backProjectImage &= HSVMask;
}

void CamshiftProcessing::CalculateHistogramForRegionOfInterest() {
    if(ROIFortracking.area() == 0)
        return;

    //ROIFortracking &= cv::Rect(0,0, CurrentFrame.cols, CurrentFrame.rows);
    ROI = cv::Mat(HueFrame, ROIFortracking);
    ROIMask = cv::Mat(HSVMask, ROIFortracking);

    const float* phranges = histogramranges;
    cv::calcHist(&ROI, 1, 0, ROIMask, histogramImage, 1, &histogramsize, &phranges);
    cv::normalize(histogramImage, histogramImage, 0, 255, cv::NORM_MINMAX);

}

void CamshiftProcessing::GetHueFrameFromHSVFrame() {
    int channel[] = {0, 0};
    HueFrame.create(HSVFrame.size(), HSVFrame.depth());
    mixChannels(&HSVFrame, 1, &HueFrame, 1, channel, 1);
}


void CamshiftProcessing::GetRegionOfInterest() {
    static int iter =0;

    if(ROIFortracking.area() == 0) {
        avgCenterPoint.x += CentroidPoint.x;
        avgCenterPoint.y += CentroidPoint.y;
        iter++;
        if(iter == 5) {
            avgCenterPoint.x /= 6;
            avgCenterPoint.y /= 6;
            ROIFortracking = cv::Rect(avgCenterPoint.x - 20, avgCenterPoint.y -20, 40, 40);
              // ROIFortracking = BoundingBoxForROIForTracking;
            ROIFortracking &= cv::Rect(0,0, CurrentFrame.cols, CurrentFrame.rows);
        }
    }
}

void CamshiftProcessing::SelectRegionOfInterest() {
    if(motionMask.empty())
        return;


     ROIClass->Centroid(CentroidPoint);
     ROIClass->FindRegionOfInterestToTrack(motionMask);
     ROIClass->DrawBoundingBoxforRegionOfInterest(CurrentFrame);
     BoundingBoxForROIForTracking = ROIClass->GetBoundingBoxOfRegionofInterest();
     GetRegionOfInterest();


}

void CamshiftProcessing::SetHSVColorProbabilityMask() {
    // Include this if the object to be tracked is of Face
    // The HSVmaskValue is obtained from training face Detection Classifier

    //cv::inRange(HSVFrame, cv::Scalar(HSVmaskValue.minhue, HSVmaskValue.minSat, HSVmaskValue.minValue),
      //      cv::Scalar(HSVmaskValue.maxhue, HSVmaskValue.maxSat, HSVmaskValue.maxvalue), HSVMask);

    cv::inRange(HSVFrame, cv::Scalar(6.0, 60.0, 32.0),
            cv::Scalar(38.0, 255.0, 255.0), HSVMask);

}

void CamshiftProcessing::ConvertBGRImageToHSV() {
    if(!CurrentFrame.empty())
        cv::cvtColor(CurrentFrame, HSVFrame, CV_BGR2HSV);
}


double Square(double value) {
    return( value * value);
}


void CamshiftProcessing::newLocationOfCenter(cv::RotatedRect& trackingBox) {
    double hypotenuse =  std::sqrt(Square(prevtrackingBox.center.x) + Square(prevtrackingBox.center.y));
    double newangle = prevtrackingBox.angle + 10;
    trackingBox.angle = newangle;
    trackingBox.center.y =  std::sin(newangle) * hypotenuse;
    trackingBox.center.x = std::cos(newangle) * hypotenuse;
}


bool CamshiftProcessing::IsAverageDistanceCalculated(cv::RotatedRect trackingBox) {
    if(IsAverageDistance)
        return true;
    if(GetAverageValueForDistance(trackingBox))
        return true;
    return false;
}

double CamshiftProcessing::GetAverageValueForDistance(cv::RotatedRect trackingBox) {
    static int iter = 0;
    if(iter == 20) {
        averageCamshiftDistance /= 20;
        IsAverageDistance = true;
        std::cout << "average distance" << averageCamshiftDistance << std::endl;
        return averageCamshiftDistance;
    }
    if(iter == 0) {
         iter++;
        return 0;
    }
    double distance = std::sqrt(Square(prevtrackingBox.center.x - trackingBox.center.x) + Square(prevtrackingBox.center.y - trackingBox.center.y));
    std::cout << ".." << distance << std::endl;
     averageCamshiftDistance += distance;
    iter++;
     return 0;
}

void CamshiftProcessing::DrawEllipseForTrackingObject(cv::RotatedRect trackingBox) {


    if(prevtrackingBox.boundingRect().area() == 0)
        prevtrackingBox = trackingBox;
    else {
        if(IsAverageDistance) {
            if(std::sqrt(Square(prevtrackingBox.center.x - trackingBox.center.x) + Square(prevtrackingBox.center.y - trackingBox.center.y)) > (averageCamshiftDistance+30)) {
                trackingBox.angle = prevtrackingBox.angle;// + 1;
                trackingBox.center.x = prevtrackingBox.center.x;
                trackingBox.center.y = prevtrackingBox.center.y;
                //newLocationOfCenter(trackingBox);
                //std::cout << trackingBox.center.x << "::" << trackingBox.center.y << std::endl;
                // calculate new position;
            }
        } else {
            IsAverageDistanceCalculated(trackingBox);
        }
    }



    CvBox2D box ;
    box.angle = trackingBox.angle;
    box.center = trackingBox.center;
    box.size = cv::Size(100,150);

    cv::RotatedRect rotatedBox = cv::RotatedRect(box);

    cv::ellipse( CurrentFrame, rotatedBox, cv::Scalar(0,0,255), 3, CV_AA );

    if(IsAverageDistance) {
    if(drawingMode) {
        cv::line(DrawingBoard, prevtrackingBox.center, trackingBox.center, cv::Scalar(0,0,255));//, 5, 8, CV_AA);
        //cv::circle(DrawingBoard, box.center, 20, cv::Scalar(0,255,0), -1, 8, CV_AA);
        cv::imshow("Drawing", DrawingBoard);
    }else {
        if(!DrawingBoard.empty()) {
            cv::imwrite("FirstDrawing.jpg", DrawingBoard);
            DrawingBoard.release();
        }
    }
    }
    std::swap(prevtrackingBox, trackingBox);

    cv::imshow("Camshift", CurrentFrame);
}

void CamshiftProcessing::DrawTrackingObject(cv::RotatedRect trackingBox) {

    if( trackingWindow.area() <= 1 )
    {
        int cols = finalProbabilityMask.cols, rows = finalProbabilityMask.rows, r = (MIN(cols, rows) + 5)/6;
        trackingWindow = cv::Rect(trackingWindow.x - r, trackingWindow.y - r,
                           trackingWindow.x + r, trackingWindow.y + r) &
                      cv::Rect(0, 0, cols, rows);
    }
    DrawEllipseForTrackingObject(trackingBox);
}

void CamshiftProcessing::InitializeCamshift() {
    ConvertBGRImageToHSV();
    SetHSVColorProbabilityMask();
    GetHueFrameFromHSVFrame();

    if(ROIFortracking.area()!=0 && IsFirstSelection) {
        CalculateHistogramForRegionOfInterest();
        trackingWindow = ROIFortracking;
        IsFirstSelection = false;
    }

}

void CamshiftProcessing::GetFinalProbabilityMask() {
    if(flowMode) {
        if(motionMask.rows != 0 && motionMask.cols !=0 )
            cv::addWeighted(backProjectImage, 0.2, motionMask, 0.9, 0.0, finalProbabilityMask);
    }
    else
        finalProbabilityMask = backProjectImage.clone();

        if( backprojMode )
            cv::cvtColor( finalProbabilityMask, CurrentFrame, CV_GRAY2BGR );
}

void CamshiftProcessing::ApplyFinalProbabilityMask() {
    if(trackingWindow.area() == 0)
        return;
     cv::RotatedRect trackingBox = cv::CamShift(finalProbabilityMask, trackingWindow, cv::TermCriteria( cv::TermCriteria::EPS | cv::TermCriteria::COUNT, 10, 1));
     DrawTrackingObject(trackingBox);
}

void CamshiftProcessing::StartCamshift() {
    InitializeCamshift();
    CalculateBackProjection();
    GetFinalProbabilityMask();
    ApplyFinalProbabilityMask();
}

void CamshiftProcessing::ActionOnKeyPress(int key) {
    switch(key)
    {
    case 'b':
        backprojMode = !backprojMode;
        break;
    case 'f':
        flowMode = !flowMode;
        break;
    case 'd' :
        drawingMode = !drawingMode;
        if(drawingMode)
            SetupDrawingBoard();
        break;
    default:
        ;
    }
}


void CamshiftProcessing::SetupDrawingBoard() {

    if(CurrentFrame.empty())
        return;
    DrawingBoard = cv::Mat::zeros(CurrentFrame.size(), CV_8UC3);
    CurrentFrame.release();
}

void CamshiftProcessing::TrackRegionOfInterest() {

    cv::Mat sample;
    GetColorProbabilityMask();

    while(true) {

        videoProcessorClass->capture.read(CurrentFrame);
        if(CurrentFrame.empty())
            continue;

        sample = CurrentFrame.clone();
        SetOpticalFlow();
        if(motionMask.empty())
            continue;
        if((flowMode)||(!flowMode && ROIFortracking.area()!=0))  {
        SelectRegionOfInterest();
        if(ROIFortracking.area() != 0)
            StartCamshift();
        }

        char c = (char)cv::waitKey(10);
        if( c == 27 )
            break;
        ActionOnKeyPress(c);

    }
}



