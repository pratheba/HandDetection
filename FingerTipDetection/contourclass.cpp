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

    cv::Mat inputFrame = inputFrame_.clone();
    cv::Mat showingFrame = cv::Mat(400,500,inputFrame.type());
    inputFrame.copyTo(showingFrame);

    cv::imshow("input", showingFrame);

    /*

    cv::Mat segmentFrame = inputFrame.clone();//inputFrame.clone();
    cv::cvtColor(segmentFrame, segmentFrame, CV_BGR2GRAY);
    cv::threshold(segmentFrame, segmentFrame, 100, 255, CV_THRESH_BINARY);
    cv::imshow("threshold", segmentFrame);
    cv::Mat segmentFrameNorm = segmentFrame.clone();
    cv::normalize(segmentFrameNorm, segmentFrameNorm, 0, 255, cv::NORM_MINMAX);
    cv::imshow("thresholdnorm", segmentFrameNorm);

    cv::Mat distanceTransformFrame;
    cv::distanceTransform(segmentFrame, distanceTransformFrame, CV_DIST_L2, 5);
    cv::normalize(distanceTransformFrame, distanceTransformFrame, 0, 1, cv::NORM_MINMAX);
    cv::imshow("distance transform", distanceTransformFrame);

    cv::threshold(distanceTransformFrame, distanceTransformFrame, 0.5, 1.0, CV_THRESH_BINARY);
    cv::imshow("distance transform 2", distanceTransformFrame);

    cv::Mat distanceTransformFrame8U;
    distanceTransformFrame.convertTo(distanceTransformFrame8U, CV_8U);

    std::vector< std::vector <cv::Point> > contours;
    cv::findContours(distanceTransformFrame8U, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    cv::Mat markers = cv::Mat::zeros(distanceTransformFrame.size(), CV_32SC1);
    for(int i=0; i< contours.size(); i++)
        cv::drawContours(markers, contours, i, cv::Scalar::all((i+1)), -1);
    cv::circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
         cv::imshow("markers", markers*10000);

     cv::watershed(inputFrame, markers);

              std::vector<cv::Vec3b> colors;
              for (int i = 0; i < contours.size(); i++)
                  {
                      int b = cv::theRNG().uniform(0, 255);
                      int g = cv::theRNG().uniform(0, 255);
                      int r = cv::theRNG().uniform(0, 255);

                      colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
                  }

                  // Create the result image
                  cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);

                  // Fill labeled objects with random colors
                  for (int i = 0; i < markers.rows; i++)
                  {
                      for (int j = 0; j < markers.cols; j++)
                      {
                          int index = markers.at<int>(i,j);
                          if (index > 0 && index <= contours.size())
                              dst.at<cv::Vec3b>(i,j) = colors[index-1];
                          else
                              dst.at<cv::Vec3b>(i,j) = cv::Vec3b(0,0,0);
                      }
                  }

                  cv::imshow("dst", dst);*/


    /*
    cv::Mat cannyOutput;
    std::vector<cv::Vec4i> hierarchy;
    std::vector< std::vector< cv::Point> > hull;

    //while(true) {
         //videoProcessorClass->capture.read(inputFrame);

         cv::cvtColor(inputFrame, inputFrame, CV_BGR2GRAY);
         cv::blur(inputFrame, inputFrame, cv::Size(3,3));

         cv::Canny(inputFrame, cannyOutput, 100, 200, 3);
         //cv::threshold(cannyOutput, cannyOutput, 150, 200, cv::THRESH_BINARY);
         cv::findContours(cannyOutput, contourPoints, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cv::Point(0,0));
         contourImage = cv::Mat(200,300, CV_8UC3,cv::Scalar(0));//cv::Mat::zeros(inputFrame_.size(), CV_8UC3);
         hull.resize(contourPoints.size());

         for(int i=0; i < contourPoints.size(); i++)
             cv::convexHull(cv::Mat(contourPoints[i]), hull[i], false);

         for(int i=0; i < contourPoints.size(); i++) {
             cv::drawContours(contourImage, contourPoints, -1, cv::Scalar(255,0,0), 2, 8, hierarchy);
             //cv::drawContours(contourImage, hull, -1, cv::Scalar(0,255,0), 2, 8, hierarchy);
         }

         ShowContourImage();

    //     int c = cvWaitKey(10);
      //   if(c==27)  //ESC key
        //     break;
  //  }
  */

}

void ContourClass::ShowContourImage(){
    cv::imshow("contour image", contourImage);
}


