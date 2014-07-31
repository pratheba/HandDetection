#include "houghtransformclass.h"

HoughTransformClass::HoughTransformClass()
{
}

void HoughTransformClass::findCircles(cv::Mat inputSource) {

  cv::Mat inputSource_gray;// = inputSource.clone();
  cv::Mat cannyOutput;

  if( !inputSource.data )
    { return; }

  std::cout << inputSource.rows << "::" << inputSource.cols << std::endl;
  std::cout << inputSource_gray.rows << "::" << inputSource_gray.cols << std::endl;

  /// Convert it to gray
  cv::cvtColor(inputSource, inputSource_gray, CV_BGR2GRAY );
  cv::Canny(inputSource, cannyOutput, 100, 200, 3);

  //cv::blur(inputFrame, cannyOutput, cv::Size(3,3));


  /// Reduce the noise so we avoid false circle detection
  cv::GaussianBlur( inputSource_gray, inputSource_gray, cv::Size(9, 9), 2,2 );
  cv::GaussianBlur( cannyOutput, cannyOutput, cv::Size(9, 9), 5,5 );


  std::vector<cv::Vec4i> lines;
    HoughLinesP(inputSource_gray, lines, 1, CV_PI/180, 100, 50, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
      cv::Vec4i l = lines[i];
      line( inputSource, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 3, CV_AA);
    }


//    std::vector<cv::Vec2f> lines;
//    HoughLines(inputSource_gray, lines, 1, CV_PI/180, 100, 0, 0 );

//    for( size_t i = 0; i < lines.size(); i++ )
//    {
//       float rho = lines[i][0], theta = lines[i][1];
//       cv::Point pt1, pt2;
//       double a = cos(theta), b = sin(theta);
//       double x0 = a*rho, y0 = b*rho;
//       pt1.x = cvRound(x0 + 1000*(-b));
//       pt1.y = cvRound(y0 + 1000*(a));
//       pt2.x = cvRound(x0 - 1000*(-b));
//       pt2.y = cvRound(y0 - 1000*(a));
//       line( inputSource, pt1, pt2, cv::Scalar(0,0,255), 3, CV_AA);
//    }


//  std::vector<cv::Vec3f> circles;

//  /// Apply the Hough Transform to find the circles
//  HoughCircles( cannyOutput, circles, CV_HOUGH_GRADIENT, 1, inputSource_gray.rows/8, 30,50, 0, 0 );

//  /// Draw the circles detected
//  for( size_t i = 0; i < circles.size(); i++ )
//  {
//      cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
//      int radius = cvRound(circles[i][2]);
//      // circle center
//      circle( inputSource, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
//      // circle outline
//      circle( inputSource, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
//   }

  /// Show your results
  cv::namedWindow( "Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE );
  cv::imshow( "Hough Circle Transform Demo", inputSource );

  cv::namedWindow( "canny Circle Transform Demo", CV_WINDOW_AUTOSIZE );
  cv::imshow( "canny Circle Transform Demo", cannyOutput );


  cv::waitKey(0);
  return ;
}
