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
    drawOpticalFlowMode = false;

    posX = 0;
    posY = 0;

    DrawingBoard = cv::Mat();
}

void CamshiftProcessing::GetColorProbabilityMask() {
    if(videoProcessorClass->facedetectionClass != NULL) {
        HSVmaskValue = videoProcessorClass->facedetectionClass->GetHSVmaskForFace();
    }
    else {//float* phranges;

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
    if(!motionMask.empty() && drawOpticalFlowMode)
        motionMaskClass->DisplayFlow();
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

    cv::inRange(HSVFrame, cv::Scalar(HSVmaskValue.minhue, HSVmaskValue.minSat, HSVmaskValue.minValue),
            cv::Scalar(HSVmaskValue.maxhue, HSVmaskValue.maxSat, HSVmaskValue.maxvalue), HSVMask);

    //cv::inRange(HSVFrame, cv::Scalar(6.0, 60.0, 32.0),
      //      cv::Scalar(38.0, 255.0, 255.0), HSVMask);

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
     averageCamshiftDistance += distance;
    iter++;
     return 0;
}

void CamshiftProcessing::DrawEllipseForTrackingObject(cv::RotatedRect trackingBox) {


    if(prevtrackingBox.boundingRect().area() == 0)
        prevtrackingBox = trackingBox;
   /* else {
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
    }*/



    CvBox2D box ;
    box.angle = trackingBox.angle;
    box.center = trackingBox.center;
    //if(BoundingBoxForROIForTracking.area() < 5000)
      //  box.size = cv::Size(30,30);
    //else
        box.size = cv::Size(100,100);

    cv::RotatedRect rotatedBox = cv::RotatedRect(box);

    cv::ellipse( CurrentFrame, rotatedBox, cv::Scalar(0,0,255), 3, CV_AA );

   // if(IsAverageDistance) {
    if(drawingMode) {
        //std::cout << BoundingBoxForROIForTracking.area() << std::endl;
        //if(BoundingBoxForROIForTracking.area() < 5000)
          //  cv::circle(DrawingBoard, trackingBox.center, 5, cv::Scalar(0),-1, 8, CV_AA);
        //else {


         cv::line(DrawingBoard, prevtrackingBox.center, trackingBox.center, cv::Scalar(0,0,255));//, 5, 8, CV_AA);
         cv::circle(DrawingBoard, trackingBox.center, 10, cv::Scalar(0,0,255), -1, 8, CV_AA);
        //}
        //cv::circle(DrawingBoard, box.center, 20, cv::Scalar(0,255,0), -1, 8, CV_AA);
        // DrawingBoard.copyTo(CurrentFrame);
         cv::addWeighted(CurrentFrame, 1, DrawingBoard, 0.5, 0.0, CurrentFrame);
        cv::imshow("DrawinginFrame", CurrentFrame);
         cv::imshow("DrawinginBoard", DrawingBoard);
    }else {
        if(!DrawingBoard.empty()) {
            cv::imwrite("FirstDrawing.jpg", DrawingBoard);
            /*cv::Mat cannyOutput;
            std::vector<cv::Vec4i> hierarchy;
            std::vector< std::vector< cv::Point> > hull;

            cv::Mat inputFrame = cv::imread("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/triangle.jpg");

                 cv::cvtColor(inputFrame, inputFrame, CV_BGR2GRAY);
                 cv::blur(inputFrame, inputFrame, cv::Size(3,3));
                 //cv::dilate(inputFrame, inputFrame, cv::Mat(), cv::Point(-1,-1), 5);

                   std::vector<std::vector<cv::Point> > contourPoints;
                   cv::Mat contourImage;
                   std::vector<cv::Point> approx;

                 cv::Canny(inputFrame, cannyOutput, 0, 255, 3);
                 cv::threshold(cannyOutput, cannyOutput, 0, 255, cv::THRESH_BINARY);
                 cv::imshow("thresh", cannyOutput);
                 cv::findContours(cannyOutput, contourPoints, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0,0));
                 contourImage = cv::Mat(cannyOutput.rows,cannyOutput.cols, CV_8UC3,cv::Scalar(0));//cv::Mat::zeros(inputFrame_.size(), CV_8UC3);
                 hull.resize(contourPoints.size());

                 for(int i=0; i < contourPoints.size(); i++)
                     cv::convexHull(cv::Mat(contourPoints[i]), hull[i], false);

                 for(int i=0; i < contourPoints.size(); i++) {
                     cv::approxPolyDP(contourPoints[i],approx,0.01* cv::arcLength(contourPoints[i],true),true);
                     std::cout << approx.size() << std::endl;

                     cv::drawContours(contourImage, contourPoints, i, cv::Scalar(255,0,0), -1, 8, hierarchy);
                 }

                 std::cout << "hull" << std::endl;
                 for(int i =0; i< hull.size(); i++) {
                     cv::approxPolyDP(hull[i],approx,0.01* cv::arcLength(hull[i],true),true);
                     std::cout << approx.size() << std::endl;

                     cv::drawContours(contourImage, hull, i, cv::Scalar(0,255,0), 2, 8, hierarchy);
                 }


                 cv::imshow("contour image", contourImage);*/


            DrawingBoard.release();
        }
    }
  //  }
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
    if(drawingMode && !DrawingBoard.empty())
        cv::circle(DrawingBoard, trackingWindow.tl(), 2, cv::Scalar(255,0,0), -1, 8, CV_AA);
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
    case 'o' :
        drawOpticalFlowMode = !drawOpticalFlowMode;
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

    int count = 0;
    cv::Mat sample;
    GetColorProbabilityMask();

    while(true) {

        videoProcessorClass->capture.read(CurrentFrame);
        count++;

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



