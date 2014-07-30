#include "hogfeatureselector.h"

HOGFeatureSelector::HOGFeatureSelector() : hogDescriptorMat(cv::Mat(35,102060,CV_32FC1)),
    TrainingClass(cv::Mat(35, 1, CV_32FC1))
{
}

HOGFeatureSelector::~HOGFeatureSelector() {
    hog_features.clear();
}

void HOGFeatureSelector::ComputeHOGFeatures(std::string trainingDataSet,int numberOfImages, typeOfImage typeOfImage_) {
    std::string fileName ; //= trainingDataSet;
    std::stringstream out;
    cv::Mat imageSource;
    static int imageNumber = 1;

    if(numberOfImages < 1) {
        std::cout << "Input Error: No images to train" << std::endl;
        return;
    }

    for(int imageNum=1; imageNum<=numberOfImages; imageNum++) {
        fileName = trainingDataSet;
        out << imageNumber;
        fileName += out.str() + ".jpg";
        out.str(std::string());

        std::cout << fileName.c_str() << std::endl;
        imageSource = cv::imread(fileName.c_str(),0);
        cv::Mat resizedImage = resizeImage(imageSource);
        ComputeHOGFeatures(resizedImage, imageNumber-1, typeOfImage_);

        std::cout << imageNumber << " size=" << hog_features.size() << std::endl;
        for (int c = 0; c < hog_features.size(); c++)
        {
            hogDescriptorMat.at<float>(imageNumber-1, c) = hog_features[c];
        }

        imageNumber++;
    }
}

cv::Mat HOGFeatureSelector::resizeImage(cv::Mat inputImage) {
    cv::Mat resizedImage;
    cv::resize(inputImage, resizedImage, cv::Size(50,50));
    return resizedImage;
}

void HOGFeatureSelector::ComputeHOGFeatures(cv::Mat resizedImageSource, int imageNumber, typeOfImage typeOfImage_) {

    hogDescriptor.compute(resizedImageSource, hog_features, cv::Size(10,10), cv::Size(50,50));

    switch(typeOfImage_) {
    case IMG_TRIANGLE :
        TrainingClass.at<float>(imageNumber, 0) = 1.0;
        break;

    case IMG_CIRCLE :
        TrainingClass.at<float>(imageNumber, 0) = 2.0;
        break;

    case IMG_NONE:
        TrainingClass.at<float>(imageNumber, 0) = 3.0;
        break;
    }
}


