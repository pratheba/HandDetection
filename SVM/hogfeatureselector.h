#ifndef HOGFEATURESELECTOR_H
#define HOGFEATURESELECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <sstream>

enum typeOfImage {
    IMG_NONE     = 3,
    IMG_TRIANGLE = 2,
    IMG_CIRCLE   = 1
};

class HOGFeatureSelector
{
public:
    HOGFeatureSelector();
    ~HOGFeatureSelector();

    void SetValuesForDescriptor();
    void ComputeHOGFeatures(std::string trainingDataSet, int numberOfImages, typeOfImage typeOfImage_);
    void ComputeHOGFeaturesForTestImage(std::string testImageFile);
    cv::Mat GetHoGDescriptor(typeOfImage typeOfImage_);

    cv::Mat hogDescriptorMat;
    cv::Mat TrainingClass;
    cv::Mat TestClassMat;

private:

    cv::HOGDescriptor hogDescriptor;
    std::vector< float > hog_features;


    cv::Mat resizeImage(cv::Mat inputImage);
    void ComputeHOGFeatures(cv::Mat trainingimage,int imageNumber, typeOfImage typeOfimage_);


};

#endif // HOGFEATURESELECTOR_H
