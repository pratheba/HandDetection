#ifndef MARKERLESSAR_H
#define MARKERLESSAR_H




/*****************************************************************************
*   Markerless AR desktop application.
******************************************************************************
*   by Khvedchenia Ievgen, 5th Dec 2012
*   http://computer-vision-talks.com
******************************************************************************
*   Ch3 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/

////////////////////////////////////////////////////////////////////
// File includes:
#include "ARDrawingContext.hpp"
#include "ARPipeline.hpp"
#include "DebugHelpers.hpp"

////////////////////////////////////////////////////////////////////
// Standard includes:
#include <opencv2/opencv.hpp>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>


class MarkerLessAR
{
public:
    MarkerLessAR();
    ~MarkerLessAR();
    int startDrawing(int imageFrame);


private:
    void processVideo(const cv::Mat& patternImage, CameraCalibration& calibration, cv::VideoCapture& capture);
    void processSingleImage(const cv::Mat& patternImage, CameraCalibration& calibration, const cv::Mat& image);
    bool processFrame(const cv::Mat& cameraFrame, ARPipeline& pipeline, ARDrawingContext& drawingCtx);




};

#endif // MARKERLESSAR_H
