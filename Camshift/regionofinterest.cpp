#include "regionofinterest.h"
#define AREATHRESHOLD 400

int thresh = 100;
int max_thresh = 255;
cv::RNG rng(12345);


RegionOfInterest::RegionOfInterest()
{
    element5 = cv::Mat(5,5, CV_8U, cv::Scalar(1));
    contourClass = new ContourClass();

}


void RegionOfInterest::GetCannyImage() {
    cannyFrame = inputFrame.clone();
    cv::blur(cannyFrame, cannyFrame, cv::Size(3,3));

    cv::Canny(cannyFrame, cannyFrame, 40, 200, 3);
    cv::threshold(cannyFrame, cannyFrame, 40, 200, cv::THRESH_BINARY);
    cv::imshow("cannyThresh", cannyFrame);
}

void RegionOfInterest::FindContoursInImage() {
    //cv::morphologyEx(cannyFrame, cannyFrame, cv::MORPH_OPEN, element5);
     cv::morphologyEx(cannyFrame, cannyFrame, cv::MORPH_CLOSE, element5);
    for(int i=0; i< 5; i++)
        cv::dilate(cannyFrame, cannyFrame, cv::Mat());
    cv::findContours(cannyFrame, contourPoints, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));

}

void RegionOfInterest::FindConvexHull() {
     contourImage = cv::Mat::zeros(cannyFrame.size(), CV_8UC3);
     convexHullPoints.resize(contourPoints.size());
     for(int i=0; i < contourPoints.size(); i++) {
         cv::convexHull(cv::Mat(contourPoints[i]), convexHullPoints[i], false);
     }
}

bool RegionOfInterest::IsContourImageLargeAndNonConvex(std::vector<cv::Point> contours) {
    //cv::approxPolyDP(cv::Mat(contours), approxContour, cv::arcLength(cv::Mat(contours), true)*0.02, true);
    cv::approxPolyDP(cv::Mat(contours), approxContour,5, true);
    if (std::fabs(cv::contourArea(contours)) < AREATHRESHOLD || !cv::isContourConvex(approxContour))
        return false;
    return true;
}

bool RegionOfInterest::IsConvexHullAreaLargeAndNonConvex(std::vector<cv::Point> convexHullPoints) {
    //cv::approxPolyDP(cv::Mat(convexHullPoints), approxConvexHull, cv::arcLength(cv::Mat(convexHullPoints), true), true);
    cv::approxPolyDP(cv::Mat(convexHullPoints), approxConvexHull, 5, true);
    if (std::fabs(cv::contourArea(convexHullPoints)) < AREATHRESHOLD || !cv::isContourConvex(approxConvexHull))
        return false;
    return true;

}

void RegionOfInterest::DrawContourAndConvexHull() {


   /* std::vector< std::vector<cv::Point> > contours_poly( contourPoints.size() );
    std::vector<cv::Rect> boundRect( contourPoints.size() );
    std::vector<cv::Point2f>center( contourPoints.size() );
    std::vector<float>radius( contourPoints.size() );*/

    contourImage = cv::Mat::zeros(cannyFrame.size(), CV_8UC3);
    for(int i=0; i < contourPoints.size(); i++) {
        if(IsContourImageLargeAndNonConvex(contourPoints[i])) {
            //cv::drawContours(contourImage, contourPoints, i, cv::Scalar(255,0,0), -1, 8, hierarchy);
            cv::drawContours(contourImage, convexHullPoints, i, cv::Scalar(0,255,0), -1, 8, hierarchy);
        }
    }

    for(int i=0; i < convexHullPoints.size(); i++) {
        if(IsConvexHullAreaLargeAndNonConvex(convexHullPoints[i])) {
            //cv::approxPolyDP(cv::Mat(convexHullPoints[i]), approxConvexHull, 5, true);
            cv::drawContours(contourImage, convexHullPoints, i, cv::Scalar(255,0,0), -1, 8, hierarchy);
    }
    }

   /* double maxArea = 0;
    int whichContour = -1;

    for( int i = 0; i < contourPoints.size(); i++ ) {
        cv::approxPolyDP( cv::Mat(contourPoints[i]), contours_poly[i], 3, true );
        double area = std::fabs(cv::contourArea(contours_poly[i]));
        if(area > maxArea) {// && area >= AREATHRESHOLD) {
            area = maxArea;
            whichContour = i;
        }
    }
     cv::Mat drawing = cv::Mat::zeros( cannyFrame.size(), CV_8UC3 );

    //if(maxArea) {
    cv::Rect bbRect = cv::boundingRect( cv::Mat(contours_poly[whichContour]) );
    cv::minEnclosingCircle( (cv::Mat)contours_poly[whichContour], center[whichContour], radius[whichContour] );


    cv::Scalar color = cv::Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
    cv::drawContours( drawing, contours_poly, whichContour, color, -1, 8, std::vector<cv::Vec4i>(), 0, cv::Point() );
    cv::rectangle( drawing, bbRect.tl(), bbRect.br(), color, 2, 8, 0 );
     cv::circle( drawing, center[whichContour], (int)radius[whichContour], color, 2, 8, 0 );
     cv::circle(drawing, center[whichContour], 5, cv::Scalar(0,0,255), -1, 8, CV_AA);



     if(ContourCenter.size() == 50) {
         cv::Mat centerOfContourImage = cv::Mat::zeros( cannyFrame.size(), CV_8UC3 );
         for(int i=1; i<50; i++) {
             //cv::circle(centerOfContourImage, ContourCenter[i], 2, cv::Scalar(0,0,255), -1, 8, CV_AA);
             cv::line(centerOfContourImage, centroidCenter[i], centroidCenter[i-1], cv::Scalar(0,255,0));//, 5, 8, CV_AA);
             //cv::circle(centerOfContourImage, centroidCenter[i], 20, cv::Scalar(0,255,0), -1, 8, CV_AA);
         }
         cv::imwrite("contourCenter.jpg", centerOfContourImage);
         ContourCenter.clear();
     }

     ContourCenter.push_back(center[whichContour]);
     centroidCenter.push_back(cen);*/


      /// Show in a window

//}
    //cv::namedWindow( "Contour-circle", CV_WINDOW_AUTOSIZE );
    //cv::imshow( "Contour-circle", drawing );
    cv::imshow("contoursAndConvexHull", contourImage);

}

void RegionOfInterest::SetmaxArea(double area, double& maxArea, std::vector<cv::Point> boundingRectPoints) {
    if(area > maxArea) {
        maxArea = area;
        boundingRect = cv::boundingRect(boundingRectPoints);
    }
}

void RegionOfInterest::FindBoundingBox() {
    double maxArea = 0;
     for(int i=0; i < convexHullPoints.size(); i++) {
         double area = cv::contourArea(convexHullPoints[i], false);
         SetmaxArea(area, maxArea, convexHullPoints[i]);
     }
}

void RegionOfInterest::GetContourImage() {
    GetCannyImage();
    FindContoursInImage();
}

void RegionOfInterest::SegmentByConvexHull() {
    if(inputFrame.empty())
       return;
    GetContourImage();
    FindConvexHull();
}

void RegionOfInterest::FindRegionOfInterestToTrack(cv::Mat inputFrame_) {
    if(inputFrame_.empty())
        return;
     inputFrame  = inputFrame_.clone();
     cv::GaussianBlur(inputFrame, inputFrame, cv::Size(5,5), 0, 0);
     cv::medianBlur(inputFrame, inputFrame, 5);
     SegmentByConvexHull();
     DrawContourAndConvexHull();
}

void RegionOfInterest::DrawBoundingBoxforRegionOfInterest(cv::Mat inputFrame_) {

    FindBoundingBox();
    cv::rectangle(inputFrame_, boundingRect, cv::Scalar(0,255,0), 1, 8, 0);

    //cv::imshow("bounding box", inputFrame_);
}

cv::Rect RegionOfInterest::GetBoundingBoxOfRegionofInterest() {
    return boundingRect;
}


void RegionOfInterest::Centroid(cv::Point cen_) {
    cen = cen_;
}
