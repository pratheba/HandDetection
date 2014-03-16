#ifndef REGIONOFINTEREST_H
#define REGIONOFINTEREST_H


#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class RegionOfInterest
{
public:
    RegionOfInterest();
    void FindRegionOfInterestToTrack();
    void DrawBoundingBoxforRegionOfInterest();

private:
};

#endif // REGIONOFINTEREST_H
