#include "regionofinterest.h"
#define AREATHRESHOLD 25


RegionOfInterest::RegionOfInterest()
{
    element5 = cv::Mat(5,5, CV_8U, cv::Scalar(1));

}


void RegionOfInterest::GetCannyImage() {
    cannyFrame = inputFrame.clone();
    cv::blur(cannyFrame, cannyFrame, cv::Size(3,3));

    cv::Canny(cannyFrame, cannyFrame, 100, 200, 3);
    cv::threshold(cannyFrame, cannyFrame, 150, 200, cv::THRESH_BINARY);
    cv::imshow("cannyThresh", cannyFrame);
}

void RegionOfInterest::FindContoursInImage() {
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
    contourImage = cv::Mat::zeros(cannyFrame.size(), CV_8UC3);
    for(int i=0; i < contourPoints.size(); i++) {
        if(IsContourImageLargeAndNonConvex(contourPoints[i])) {
            //cv::drawContours(contourImage, contourPoints, i, cv::Scalar(255,0,0), -1, 8, hierarchy);
            cv::drawContours(contourImage, convexHullPoints, i, cv::Scalar(0,255,0), 2, 8, hierarchy);
        }
    }

    for(int i=0; i < convexHullPoints.size(); i++) {
        if(IsConvexHullAreaLargeAndNonConvex(convexHullPoints[i])) {
            //cv::approxPolyDP(cv::Mat(convexHullPoints[i]), approxConvexHull, 5, true);
            cv::drawContours(contourImage, convexHullPoints, i, cv::Scalar(255,0,0), -1, 8, hierarchy);
    }
    }
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
     SegmentByConvexHull();
     DrawContourAndConvexHull();
}

void RegionOfInterest::DrawBoundingBoxforRegionOfInterest(cv::Mat inputFrame_) {

    FindBoundingBox();
    cv::rectangle(inputFrame_, boundingRect, cv::Scalar(0,255,0), 1, 8, 0);
    cv::imshow("bounding box", inputFrame_);
}

cv::Rect RegionOfInterest::GetBoundingBoxOfRegionofInterest() {
    return boundingRect;
}
