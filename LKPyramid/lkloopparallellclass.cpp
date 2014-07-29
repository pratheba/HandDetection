#include "lkloopparallellclass.h"

LKLoopParallellClass:: LKLoopParallellClass(cv::Mat& OpticalFlowImage_, cv::Mat& error_,std::vector<cv::Point2f>& currentFeatures, std::vector<cv::Point2f>& prevFeatures, std::vector<uchar>& statusOfFlowForFeature_):
    CurrentFramefeatures(currentFeatures), PrevFramefeatures(prevFeatures), OpticalFlowImage(OpticalFlowImage_), error(error_), statusOfFlowForFeature(statusOfFlowForFeature_)
{
}


LKLoopParallellClass::~LKLoopParallellClass() {

}


//LKLoopParallellClass::LKLoopParallellClass(LKLoopParallellClass& lkloopclass): CurrentFramefeatures(lkloopclass.CurrentFramefeatures),
//    PrevFramefeatures(lkloopclass.PrevFramefeatures), OpticalFlowImage(lkloopclass.OpticalFlowImage), statusOfFlowForFeature(lkloopclass.statusOfFlowForFeature),
//    error(lkloopclass.error)
//{
//}

double LKLoopParallellClass::GetAngleOfFlow(cv::Point2f PrevPt, cv::Point2f currPt) const {
    double angle;
    angle = atan2( (double) PrevPt.y - currPt.y, (double) PrevPt.x - currPt.x );
    return angle;
}

double LKLoopParallellClass::getLengthOfFlow(cv::Point2f PrevPt, cv::Point2f currPt) const{
    double hypotenuse;
    hypotenuse = sqrt( std::pow((PrevPt.y - currPt.y),2) + std::pow((PrevPt.x - currPt.x),2) );
    return hypotenuse;
}

void LKLoopParallellClass::DrawTheArrow(cv::Point2f PrevPt, cv::Point2f currPt, double angle) const{
    cv::line(OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0);

     PrevPt.x = (int) (currPt.x + 5 * cos(angle + (3.14) / 4));
     PrevPt.y = (int) (currPt.y + 5 * sin(angle + (3.14) / 4));
     cv::line( OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0 );
     PrevPt.x = (int) (currPt.x + 5 * cos(angle - (3.14) / 4));
     PrevPt.y = (int) (currPt.y + 5 * sin(angle - (3.14) / 4));
     cv::line( OpticalFlowImage, PrevPt, currPt, CV_RGB(255, 0, 0), 1, CV_AA, 0 );
}

void LKLoopParallellClass::operator()(const tbb::blocked_range<int>& range) const
{
    std::vector < cv::Point2f > currentfeatures = CurrentFramefeatures;
    std::vector < cv::Point2f > prevFeatures    = PrevFramefeatures;


    for(int i= range.begin(); i< range.end();i++) {
        if ((statusOfFlowForFeature[i] == 0) || (int)error.data[i] > 550)
            continue;

          cv::Point2f PrevPt = cv::Point2f(cvRound(prevFeatures[i].x), cvRound(prevFeatures[i].y));
          cv::Point2f currPt = cv::Point2f(cvRound(currentfeatures[i].x), cvRound(currentfeatures[i].y));
          double angle = GetAngleOfFlow(PrevPt, currPt);
          double hypotenuse = getLengthOfFlow(PrevPt, currPt);

          currPt.x = (int) (PrevPt.x - 3 * hypotenuse * cos(angle));
          currPt.y = (int) (PrevPt.y - 3 * hypotenuse * sin(angle));
          DrawTheArrow(PrevPt, currPt, angle);
    }
}
