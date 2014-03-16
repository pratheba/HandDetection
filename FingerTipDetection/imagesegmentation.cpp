#include "imagesegmentation.h"

// Use of watershed algorthm to seperate out hand region
ImageSegmentation::ImageSegmentation()
{
    videoProcessorClass = VideoProcessorClass::getInstance();
}

 void ImageSegmentation::FindDistanceTransformation(cv::Mat& inputFrame) {
     //UNUSED_PARAMETER(inputFrame);

 }

 void ImageSegmentation::SegmentByWaterShed(cv::Mat inputFrame_) {

     cv::Mat inputFrame;
     cv::Mat element5(5,5, CV_8U, cv::Scalar(1));
     //while(true) {

         //videoProcessorClass->capture.read(inputFrame);
     std::stringstream ss;
     std::vector< std::vector< cv::Point> > hull;


     for(int iter=0; iter < 10; iter++) {
         ss.str().clear();
         ss.str(std::string());

         ss << iter;
         std::string inputImages = std::string("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/images/Output/frame")+std::string(ss.str())+std::string(".jpg");


         inputFrame = cv::imread(inputImages);

         if(inputFrame.empty())
             continue;
         inputImages.clear();


         cv::Mat cannyFrame = inputFrame.clone();
         cv::blur(cannyFrame, cannyFrame, cv::Size(3,3));
           //cv::morphologyEx(cannyFrame, cannyFrame, cv::MORPH_OPEN, element5);
        // cv::morphologyEx(cannyFrame, cannyFrame, cv::MORPH_CLOSE, element5);


         cv::Canny(cannyFrame, cannyFrame, 100, 200, 3);
         cv::threshold(cannyFrame, cannyFrame, 150, 200, cv::THRESH_BINARY);
         cv::imshow("cannyThresh", cannyFrame);

         std::vector< std::vector < cv::Point> > contourPoints;
          std::vector<cv::Vec4i> hierarchy;

          cv::morphologyEx(cannyFrame, cannyFrame, cv::MORPH_CLOSE, element5);

          //cv::morphologyEx(cannyFrame, cannyFrame, cv::MORPH_OPEN, element5);

         cv::findContours(cannyFrame, contourPoints, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0,0));
         cv::Mat contourImage = cv::Mat::zeros(cannyFrame.size(), CV_8UC3);
         hull.resize(contourPoints.size());
         std::vector<std::vector<cv::Point> > hullContourPoints;
         double maxarea = 0;
         int whichhull = -1;
         hullContourPoints.resize(contourPoints.size());

         for(int i=0; i < contourPoints.size(); i++) {
             cv::convexHull(cv::Mat(contourPoints[i]), hull[i], false);
            /* cv::approxPolyDP(cv::Mat(hull[i]), hullContourPoints[i], 0.001, true);
             double a = std::fabs(cv::contourArea(cv::Mat(hullContourPoints[i])));
             std::cout << a << std::endl;

             if(a > maxarea) {
                 maxarea = a;
                 whichhull = i;
             }*/
         }

         int largestcontour = -1;
         cv::Rect boundingRect;
         for(int i=0; i < contourPoints.size(); i++) {
             //cv::drawContours(contourImage, contourPoints, -1, cv::Scalar(255,0,0), -1, 8, hierarchy);
             //if(i == whichhull){
               //  cv::drawContours(contourImage, hull, -1, cv::Scalar(0,255,0), 2, 8, hierarchy);
                // break;
             //}

             double a = cv::contourArea(contourPoints[i], false);
             if(a > maxarea) {
                 maxarea = a;
                 largestcontour = i;
                 boundingRect = cv::boundingRect(contourPoints[i]);
             }


         }

         cv::drawContours(contourImage, contourPoints, largestcontour, cv::Scalar(255,255,255), -1, 8, hierarchy);
         cv::rectangle(inputFrame, boundingRect, cv::Scalar(0,255,0), 1, 8, 0);

         imshow("contour Image", contourImage);
         imshow("bounding rect", inputFrame);


         //ShowContourImage();



     cv::Mat segmentFrame = contourImage.clone();//inputFrame.clone();
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

     //cv::circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
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

         cv::imshow("dst", dst);

         cv::waitKey(0);

         //int c = cvWaitKey(10);
         //if(c==27)  //ESC key
           //  break;

         //cv::destroyAllWindows();
     }
         //return 0;

 }
