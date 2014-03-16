#ifndef REGIONOFINTEREST_H
#define REGIONOFINTEREST_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class RegionOfInterest
{
public:
    RegionOfInterest();
    void FindRegionOfInterestToTrack(cv::Mat inputFrame_);
    void DrawBoundingBoxforRegionOfInterest(cv::Mat inputFrame);
    cv::Rect GetBoundingBoxOfRegionofInterest();

private:
    cv::Mat inputFrame;
    cv::Mat element5;
    cv::Mat contourImage;
    cv::Mat cannyFrame;
    cv::Rect boundingRect;
    std::vector< std::vector < cv::Point> > contourPoints;
    std::vector< std::vector < cv::Point> > approxContourPoints;
    std::vector< std::vector < cv::Point> > convexHullPoints;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Point> approxContour;
    std::vector<cv::Point> approxConvexHull;

    void GetCannyImage();
    void FindContoursInImage();
    void FindConvexHull();
    bool IsContourImageLargeAndNonConvex(std::vector<cv::Point> contours);
    bool IsConvexHullAreaLargeAndNonConvex(std::vector<cv::Point> convexHullPoints);
    void DrawContourAndConvexHull();
    void SetmaxArea(double area,double& maxArea, std::vector<cv::Point> boundingRectPoints);
    void FindBoundingBox();
    void GetContourImage();
    void SegmentByConvexHull();

};

#endif // REGIONOFINTEREST_H
