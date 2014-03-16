#include "contourclass.h"

ContourClass::ContourClass():contourImage(cv::Mat()), contourInverse(cv::Mat())
{
    videoProcessorClass = VideoProcessorClass::getInstance();
    cv::namedWindow("contour image");
}

ContourClass::ContourClass(cv::Mat inputFrame):contourImage(inputFrame.clone())
{
}

bool ContourClass::IsImageAvailableForCountourDetection() {
    return( !(contourImage.empty()));
}


void ContourClass::FindContourImage(cv::Mat inputFrame_){

   // if(inputFrame.empty())
     //   return;

    cv::Mat inputFrame;
    cv::Mat cannyOutput;
    std::vector<cv::Vec4i> hierarchy;
    std::vector< std::vector< cv::Point> > hull;

    while(true) {
         videoProcessorClass->capture.read(inputFrame);

         cv::cvtColor(inputFrame, inputFrame, CV_BGR2GRAY);
         cv::blur(inputFrame, inputFrame, cv::Size(3,3));

         //cv::Canny(inputFrame, cannyOutput, 100, 200, 3);
         cv::threshold(inputFrame, cannyOutput, 150, 200, cv::THRESH_BINARY);
         cv::findContours(cannyOutput, contourPoints, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0,0));
         contourImage = cv::Mat::zeros(cannyOutput.size(), CV_8UC3);
         hull.resize(contourPoints.size());

         for(int i=0; i < contourPoints.size(); i++)
             cv::convexHull(cv::Mat(contourPoints[i]), hull[i], false);

         for(int i=0; i < contourPoints.size(); i++) {
             cv::drawContours(contourImage, contourPoints, -1, cv::Scalar(255,0,0), 2, 8, hierarchy);
             //cv::drawContours(contourImage, hull, -1, cv::Scalar(0,255,0), 2, 8, hierarchy);
         }

         ShowContourImage();

         int c = cvWaitKey(10);
         if(c==27)  //ESC key
             break;
    }

}

void ContourClass::ShowContourImage(){
    cv::imshow("contour image", contourImage);
}


