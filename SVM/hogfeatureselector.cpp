#include "hogfeatureselector.h"

HOGFeatureSelector::HOGFeatureSelector() : hogDescriptorMat(cv::Mat(73,102060,CV_32FC1)),
    TrainingClass(cv::Mat(73, 1, CV_32FC1)), TestClassMat(cv::Mat(1,102060,CV_32FC1))
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

        hog_features.clear();

        fileName = trainingDataSet;
        out << imageNumber;
        fileName += out.str() + ".jpg";
        out.str(std::string());

        std::cout << fileName.c_str() << std::endl;
        imageSource = cv::imread(fileName.c_str(),0);

        //std::cout << imageSource.rows << "::" << imageSource.cols << std::endl;
        cv::Mat resizedImage = resizeImage(imageSource);
        ComputeHOGFeatures(resizedImage, imageNumber-1, typeOfImage_);

        //std::cout << imageNumber << " size=" << hog_features.size() << std::endl;
        for (int c = 0; c < hog_features.size(); c++)
        {
            hogDescriptorMat.at<float>(imageNumber-1, c) = hog_features[c];
        }

        imageNumber++;
    }
}

cv::Mat HOGFeatureSelector::resizeImage(cv::Mat inputImage) {
    cv::Mat resizedImage;
    cv::resize(inputImage, resizedImage, cv::Size(48,64));
    return resizedImage;
}

void HOGFeatureSelector::ComputeHOGFeatures(cv::Mat resizedImageSource, int imageNumber, typeOfImage typeOfImage_) {

    hogDescriptor.compute(resizedImageSource, hog_features, cv::Size(20,20), cv::Size(48,64));

    switch(typeOfImage_) {
    case IMG_TRIANGLE :
        TrainingClass.at<float>(imageNumber, 0) = 2.0;
        break;

    case IMG_CIRCLE :
        TrainingClass.at<float>(imageNumber, 0) = 1.0;
        break;

    case IMG_NONE:
        TrainingClass.at<float>(imageNumber, 0) = 3.0;
        break;
    }
}



void HOGFeatureSelector::ComputeHOGFeaturesForTestImage(std::string testImageFile) {

    std::stringstream out;
    cv::Mat testImage;

    std::string fileName = testImageFile;
    //out << "triangle.jpg";
    out << "FirstDrawing.jpg";
    //out << "28.jpg";

    fileName += out.str();

    std::cout << fileName.c_str() << std::endl;

    testImage = cv::imread(fileName.c_str(),0);
    std::cout << testImage.rows << " :: " << testImage.cols << std::endl;
    cv::Mat resizedImage = resizeImage(testImage);
    hog_features.clear();
    hogDescriptor.compute(resizedImage, hog_features, cv::Size(20,20), cv::Size(48,64));

    for (int c = 0; c < hog_features.size(); c++)
        TestClassMat.at<float>(0,c) = hog_features[c];

}
