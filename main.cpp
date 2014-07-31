/*
 * Copyright (c) 2011. Philipp Wagner <bytefish[at]gmx[dot]de>.
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 */


#include <iostream>
#include "VideoProcessing/videoprocessorclass.h"
#include "Camshift/camshiftprocessing.h"
#include "FingerTipDetection/contourclass.h"
#include "FingerTipDetection/imagesegmentation.h"
#include "SVM/hogfeatureselector.h"
#include "SVM/svmclassifier.h"
#include "SVM/houghtransformclass.h"

#include "DrawingMarkerlessAR/markerlessar.h"


int main(int argc, const char *argv[]) {

   /* if(argc < 2) {
        std::cout << " Input Arguments required " << std::endl;
        return -1;
    }*/

    std::cout<< atoi(argv[1]) << std::endl;
    VideoProcessorClass* videoProcessor  =  VideoProcessorClass::getInstance(atoi(argv[1]));// new VideoProcessorClass(atoi(argv[1]));
    CamshiftProcessing* camshift = new CamshiftProcessing();
    HOGFeatureSelector* hogfeatureSelector = new HOGFeatureSelector();
    SVMClassifier* svmClassifier = new SVMClassifier();
    //HoughTransformClass* houghClass = new HoughTransformClass();


//    MarkerLessAR* markerlessAR = new MarkerLessAR();
//    markerlessAR->startDrawing(1);



//    cv::Mat inputSource = cv::imread("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/Images/Test/circle.jpg",CV_LOAD_IMAGE_COLOR);

//    houghClass->findCircles(inputSource);

    hogfeatureSelector->ComputeHOGFeatures("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/Images/Training/Triangle/", 25, IMG_TRIANGLE);
    hogfeatureSelector->ComputeHOGFeatures("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/Images/Training/Non/", 10, IMG_NONE);
    hogfeatureSelector->ComputeHOGFeatures("/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/Images/Training/Circle/", 38, IMG_CIRCLE);


    /* Train the images to classify as triangle or circle using SVM classifier */
    svmClassifier->TrainData(hogfeatureSelector->hogDescriptorMat, hogfeatureSelector->TrainingClass);




    /* Compute features for SVM classifier using HOG feature selector */

    /* Get the VideoProcessing */
    videoProcessor->SetVideoCaptureInstance();
    //videoProcessor->ChooseProcessorClass(atoi(argv[1]));
    videoProcessor->SetframeToStopProcessing(30);
    videoProcessor->displayInput("Input Frame");
    videoProcessor->displayOutput("Output Frame");


////    // To get the time taken in terms of second



    double initialTickCount = (double)cv::getTickCount();
    videoProcessor->ProcessVideoFrame();

    double finalTickCount = (double)cv::getTickCount();

    double timeextended = (finalTickCount - initialTickCount)/cv::getTickFrequency();

    std::cout << timeextended << std::endl;

    /* Get the color Probability mask */
    camshift->TrackRegionOfInterest();

    std::string testImagefileName = "/home/pratheba/workspace/QTProject/HandDetectionWithCamshift/Images/Test/";

    hogfeatureSelector->ComputeHOGFeaturesForTestImage(testImagefileName);
    svmClassifier->ClassifyImage(hogfeatureSelector->TestClassMat);


    videoProcessor->release();
    delete camshift;
    delete hogfeatureSelector;
    delete svmClassifier;
   // delete houghClass;

    //delete markerlessAR;

    return 0;
}
